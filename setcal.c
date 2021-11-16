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
} vec_t;

void vec_print(vec_t *v, char mode){
  printf("%c ", mode);
  for(unsigned int i = 0; i < v->used; i++)
      printf("%s ", v->arr[i]);
  printf("\n");
}

int vec_constructor(vec_t *v){
  v->arr = malloc(ALLOC_CONST * sizeof(char) * MAX_ELEM_LEN);
  if (v->arr == NULL){
    // malloc failed
    return 0;
  }
  v->size = ALLOC_CONST;
  v->used = 0;
  return 1;
}

int vec_append(vec_t *v, char* elem){
  if (v->used == v->size) {
    v->arr = realloc(v->arr, (v->size + ALLOC_CONST) * sizeof(char) * MAX_ELEM_LEN);
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
  v->size = 0;
  v->used = 0;
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
} rel_t;

void rel_print(rel_t *r){
  printf("R ");
  for(unsigned int i = 0; i < r->used; i++)
      printf("(%s %s) ", r->arr[i].x, r->arr[i].y);
  printf("\n");
}

int rel_constructor(rel_t *r){
  r->arr = malloc(ALLOC_CONST * 2 * sizeof(char) * MAX_ELEM_LEN);
  if (r->arr == NULL){
    // malloc failed
    return 0;
  }
  r->size = ALLOC_CONST;
  r->used = 0;
  return 1;
}

int rel_append(rel_t *r, char* elem1, char* elem2){
  if (r->used == r->size){
    r->arr = realloc(r->arr, (r->size + ALLOC_CONST) * 2 * sizeof(char) * MAX_ELEM_LEN);
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

int main(int argc, char *argv[]){
  FILE *fp;
  fp = process_args(argc, argv);
  if (fp == NULL){
    // TODO throw err "could not load file"
    return 1;
  }
  
  // vec_t universe;
  // vec_init(&universe);
  
  int line_type = get_line_type(fp);
  while(line_type){
    switch (line_type){
      case S:
        // TODO new set
        break;
      case R:
        // TODO new relation
        break;
      case C:
        // TODO new command
        break;
      case U:
        // TODO set universe
        break;
    }
    line_type = get_line_type(fp);
  }
  // TODO handle EOF
  fclose(fp);

  vec_t v;
  if (!vec_constructor(&v)){
    // throw err "Vector construction failed"
    return 1;
  }
  vec_append(&v, "test");
  vec_append(&v, "name");
  vec_append(&v, "Jakub");
  vec_append(&v, "ruka");
  vec_print(&v, 'S');
  vec_destructor(&v);

  rel_t r;
  if (!rel_constructor(&r)){
    // throw err "Relation construction failed"
    return 1;
  }
  rel_append(&r, "test", "retest");
  rel_append(&r, "name", "surname");
  rel_append(&r, "Jakub", "Dugovic");
  rel_append(&r, "ruka", "noha");
  rel_print(&r);
  rel_destructor(&r);

  return 0;
}