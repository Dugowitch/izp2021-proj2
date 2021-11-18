#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ELEM_LEN 31
#define ALLOC_CONST 10
enum {ERR, U, S, R, C};

/**
 * @brief vector type and basic vector funcitons
 */

typedef struct {
  char **arr;
  unsigned int used;
  unsigned int size;
  unsigned int line;
} vec_t;

void vec_print(vec_t *v, char mode){
  printf("%c ", mode);
  for(unsigned int i = 0; i < v->used; i++)
      printf("%s ", v->arr[i]);
  printf("\n");
}

int vec_constructor(vec_t *v, unsigned int line){
  v->arr = malloc(ALLOC_CONST * sizeof(char) * MAX_ELEM_LEN);
  if (v->arr == NULL){
    // malloc failed
    return 0;
  }
  v->size = ALLOC_CONST;
  v->used = 0;
  v->line = line;
  return 1;
}

int vec_append(vec_t *v, char* elem){
  if (v->used == v->size) {
    v->arr = realloc(v->arr, (v->size + ALLOC_CONST) * sizeof(char) * MAX_ELEM_LEN); // FIXME ??? use temporary variable and check if realloc is successful
    if (v->arr == NULL){
      // realloc failed
      return 0;
    }
    v->size += ALLOC_CONST;
  }
  v->arr[v->used] = elem;
  v->used++;
  return 1;
}

void vec_destructor(vec_t *v){
  if (v->arr != NULL){
    free(v->arr);
    v->arr = NULL;
  }
}

/**
 * @brief relation type and basic relation functions
 */

typedef struct {
  char *x;
  char *y;
} tuple_t;

typedef struct {
  tuple_t *arr;
  unsigned int used;
  unsigned int size;
  unsigned int line;
} rel_t;

void rel_print(rel_t *r){
  printf("R ");
  for(unsigned int i = 0; i < r->used; i++)
      printf("(%s %s) ", r->arr[i].x, r->arr[i].y);
  printf("\n");
}

int rel_constructor(rel_t *r, unsigned int line){
  r->arr = malloc(ALLOC_CONST * 2 * sizeof(char) * MAX_ELEM_LEN);
  if (r->arr == NULL){
    // malloc failed
    return 0;
  }
  r->size = ALLOC_CONST;
  r->used = 0;
  r->line = line;
  return 1;
}

int rel_append(rel_t *r, char* elem1, char* elem2){
  if (r->used == r->size){
    r->arr = realloc(r->arr, (r->size + ALLOC_CONST) * 2 * sizeof(char) * MAX_ELEM_LEN); // FIXME ??? use temporary variable and check if realloc is successful
    if (r->arr == NULL){
      // realloc failed
      return 0;
    }
    r->size += ALLOC_CONST;
  }
  r->arr[r->used].x = elem1;
  r->arr[r->used].y = elem2;
  r->used++;
  return 1;
}

void rel_destructor(rel_t *r){
  if (r->arr != NULL){
    free(r->arr);
    r->arr = NULL;
  }
}

/**
 * @brief TOP LEVEL DATA STRUCTURES
 */

/**
 * @brief vector of vectors
 */

typedef struct {
  vec_t *arr;
  unsigned used;
  unsigned size;
} vvec_t;

int vvec_constructor(vvec_t *v){
  v->arr = malloc(ALLOC_CONST * sizeof(vec_t));
  if (v->arr == NULL){
    // malloc failed
    return 0;
  }
  v->size = ALLOC_CONST;
  v->used = 0;
  return 1;
}

void vvec_destructor(vvec_t *v){
  if (v->arr != NULL){
    free(v->arr);
    v->arr = NULL;
  }
  v->size = 0;
  v->used = 0;
}

/**
 * @brief vector of relations
 */

typedef struct {
  rel_t *arr;
  unsigned used;
  unsigned size;
} vrel_t;

int vrel_constructor(vrel_t *r){
  r->arr = malloc(ALLOC_CONST * sizeof(rel_t));
  if (r->arr == NULL){
    // malloc failed
    return 0;
  }
  r->size = ALLOC_CONST;
  r->used = 0;
  return 1;
}

void vrel_destructor(vrel_t *r){
  if (r->arr != NULL){
    free(r->arr);
    r->arr = NULL;
  }
  r->size = 0;
  r->used = 0;
}

/**
 * @brief parse arguments and return file pointer
 */

FILE *process_args(int argc, char *argv[]){
  if (argc > 1){
    return fopen(argv[1], "r");
  }
  // TODO ??? handle (argc == 1) => no file entered
  return NULL;
}

/**
 * @brief read first letter on a line
 */

int get_line_type(FILE *f){
  char c;
  if ((c = fgetc(f)) != EOF){
    if (c == 'U'){
      return U;
    }
    else if (c == 'S'){
      return S;
    }
    else if (c == 'R'){
      return R;
    }
    else if (c == 'C'){
      return C;
    } else if (iscntrl(c)){
      get_line_type(f);
    } else {
      // TODO handle invalid input
    }
  }
  return 0;
}

/**
 * @brief populate vector from file
 */

int new_vec(vec_t *v, FILE *f){
  char c;
  while ((c = fgetc(f)) != EOF && !iscntrl(c)){
    char *s = malloc(MAX_ELEM_LEN * sizeof(char)); // FIXME free somewhere
    fscanf(f, "%s", s);
    vec_append(v, s);
  }
  return 0;
}

/**
 * @brief populate relaiton from file
 */

int new_rel(rel_t *r, FILE *f){
  char c;
  while ((c = fgetc(f)) != EOF && !iscntrl(c)){
    char *s1 = malloc(MAX_ELEM_LEN * sizeof(char)); // FIXME free somewhere
    char *s2 = malloc(MAX_ELEM_LEN * sizeof(char)); // FIXME free somewhere
    fscanf(f, "(%s %s", s1, s2);
    s2[strlen(s2) - 1] = '\0';
    rel_append(r, s1, s2);
  }
  return 0;
}

int main(int argc, char *argv[]){
  FILE *fp;
  fp = process_args(argc, argv);
  if (fp == NULL){
    // TODO throw err "could not load file"
    return 1;
  }
  
  vec_t universe;
  vec_constructor(&universe, 0);

  vrel_t relations;
  if (!vrel_constructor(&relations)){
    // TODO handle vector of relations malloc failed
    return 1;
  }
  if (!rel_constructor(&(relations.arr[0]), 0)){
    // TODO handle relation malloc failed
    return 1;
  }
  
  int line_type = get_line_type(fp);
  while(line_type){
    switch (line_type){
      case S:
        // TODO new vector
        break;
      case R:
        // TODO new relation
        new_rel(&relations.arr[0], fp);
        break;
      case C:
        // TODO new command
        break;
      case U:
        new_vec(&universe, fp);
        vec_print(&universe, 'U');
        break;
    }
    line_type = get_line_type(fp);
  }
  // TODO handle EOF
  vec_destructor(&universe);

  vvec_t vectors;
  if (!vvec_constructor(&vectors)){
    // TODO handle vector malloc failed
    return 1;
  }
  
  // if (!vec_constructor(&(vectors.arr[0]), 0)){
  //   // throw err "Vector construction failed"
  //   return 1;
  // }
  // vec_append(&vectors.arr[0], "test");
  // vec_append(&vectors.arr[0], "name");
  // vec_append(&vectors.arr[0], "Jakub");
  // vec_append(&vectors.arr[0], "ruka");
  // vec_print(&vectors.arr[0], 'S');
  // vec_destructor(&vectors.arr[0]);

  // rel_append(&relations.arr[0], "test", "retest");
  // rel_append(&relations.arr[0], "name", "surname");
  // rel_append(&relations.arr[0], "Jakub", "Dugovic");
  // rel_append(&relations.arr[0], "ruka", "noha");
  rel_print(&relations.arr[0]);
  rel_destructor(&relations.arr[0]);

  vvec_destructor(&vectors);
  vrel_destructor(&relations);
  fclose(fp);
  return 0;
}