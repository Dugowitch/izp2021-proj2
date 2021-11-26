// IZP 2021/2020
// Projekt 2 - Práce s datovými strukturami

// Name Holder (xlogin00) //TODO
// Jakub Dugovič (xdugov00)
// Verevkin Aleksandr (xverev00)
// Martin Hrdlička (xhrdli15)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ELEM_LEN 31
#define ALLOC_CONST 10
enum {ERR, U, S, R, C};

/**
 * @brief vector structure
 */
typedef struct {
  char **arr;
  unsigned int used;
  unsigned int size;
  unsigned int line;
} vec_t;

/**
 * @brief function that print vector
 * @param v vector to print
 * @param mode type of vector (U, S)
 */
void vec_print(vec_t *v, char mode){
  printf("%c ", mode);
  for(unsigned int i = 0; i < v->used; i++)
      printf("%s ", v->arr[i]);
  printf("\n");
}

/**
 * @brief function that allocate new vector
 * @param v vector to allocate
 * @param line line of vector
 * @return 1 in case of success, 0 otherwise
 */
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

/**
 * @brief function that append new element to vector
 * @param v appended vector
 * @param elem element to append
 * @return 1 in case of success, 0 otherwise
 */
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

/**
 * @brief function that frees allocated vector
 * @param v vector to free
 */
void vec_destructor(vec_t *v){
  if (v->arr != NULL){
    free(v->arr);
    v->arr = NULL;
  }
}

/**
 * @brief single tuple relation structure
 */
typedef struct {
  char *x;
  char *y;
} tuple_t;

/**
 * @brief relation structure
 */
typedef struct {
  tuple_t *arr;
  unsigned int used;
  unsigned int size;
  unsigned int line;
} rel_t;

/**
 * @brief function that print relation
 * @param r relation to print
 */
void rel_print(rel_t *r){
  printf("R ");
  for(unsigned int i = 0; i < r->used; i++)
      printf("(%s %s) ", r->arr[i].x, r->arr[i].y);
  printf("\n");
}

/**
 * @brief function that allocate space for new relation
 * @param r relation to allocate
 * @param line line of relation
 * @return 1 in case of success, 0 otherwise
 */
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

/**
 * @brief function that append new relation tuple to relation
 * @param r appended relation
 * @param elem1 first element of appended relation
 * @param elem2 second element of appended relation
 * @return 1 in case of success, 0 otherwise
 */
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

/**
 * @brief function that frees allocated relation
 * @param r relation to free
 */
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
 * @brief vector of vectors struct
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
 * @param argc count of arguments
 * @param argv arguments
 * @return pointer on file in case of right amount of arguments, else NULL
 */
FILE *process_args(int argc, char *argv[]){
  //check if amount of arguments != 2
  if (argc != 2) {
    return NULL;
  } else {
    return fopen(argv[1], "r");
  }
}

/**
 * @brief read first letter on a line
 * @param var
 * @param f input file
 * @return 1 in case of success, 0 otherwise
 */
int get_line_type(int *var, FILE *f){
  char c;
  if ((c = fgetc(f)) != EOF){
    if (c == 'U'){
      *var = U;
    }
    else if (c == 'S'){
      *var = S;
    }
    else if (c == 'R'){
      *var = R;
    }
    else if (c == 'C'){
      *var = C;
    } else if (iscntrl(c)){
      get_line_type(var, f);
    } else {
      // TODO handle invalid input
    }
    return 1;
  }
  return 0;
}

/**
 * @brief populate vector from file
 * @param v vector to populate
 * @param f input file
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
 * @param r relation to populate
 * @param f input file
 */
int new_rel(rel_t *r, FILE *f){
  char c;
  while ((c = fgetc(f)) != EOF && !iscntrl(c)){
    char *s1 = malloc(MAX_ELEM_LEN * sizeof(char)); // FIXME free somewhere
    char *s2 = malloc(MAX_ELEM_LEN * sizeof(char)); // FIXME free somewhere
    fscanf(f, "(%s %s", s1, s2);
    s2[strlen(s2) - 1] = '\0'; //removes unwanted bracket
    rel_append(r, s1, s2);
  }
  return 0;
}

/* ======================================= RELATION OPERATIONS START ======================================= */
/**
 * @brief find if relation on given line exist
 * @param relations relation to find
 * @param line line of relation
 * @return array index of relation in case of success, else -1
 */
int find_relation(vrel_t *relations, unsigned int line) {
  for (unsigned int i = 0; i <= 10; i++) {                  //TODO cycle until what?
    if (relations->arr[i].line == line - 1) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief check if relation is reflexive
 * @param universe universe(U) array
 * @param relations vector of relations
 * @param arr_position index of relation in array
 */
void reflexive_rel(vec_t *universe, vrel_t *relations, int arr_position) {
  //check if duplicate relation of every element in universe exist
  for (unsigned int i = 0; i < universe->used; i++) {
    int ref = 0;
    for (unsigned int j = 0; j < relations->arr[arr_position].used; j++) {
      if (!(strcmp(universe->arr[i], relations->arr[arr_position].arr[j].x)) &&
          !(strcmp(universe->arr[i], relations->arr[arr_position].arr[j].y))) {
            ref = 1;
            break;
      }
    }
    //print false in case of missing something
    if (ref != 1) {
      printf("false\n");
      return;
    }
  }
  printf("true\n");
}

/**
 * @brief check if relation is symmetric
 * @param relations vector of relations
 * @param arr_position index of relation in array
 */
void symmetric_rel(vrel_t *relations, int arr_position) {
  for (unsigned int i = 0; i < relations->arr[arr_position].used; i++) {
    //go for another iteration, if elements of relation are equal
    if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[i].y))) {
      continue;
    }
    int sym = 0;
    for (unsigned int j = 0; j < relations->arr[arr_position].used; j++) {
      if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[j].y)) &&
          !(strcmp(relations->arr[arr_position].arr[i].y, relations->arr[arr_position].arr[j].x))) {
            sym = 1;
            break;
      }
    }
    //print false in case of missing something
    if (sym != 1) {
      printf("false\n");
      return;
    }
  }
  printf("true\n");
}

/**
 * @brief check if relation is antisymmetric
 * @param relations vector of relations
 * @param arr_position index of relation in array
 */
void antisymmetric_rel(vrel_t *relations, int arr_position) {
  for (unsigned int i = 0; i < relations->arr[arr_position].used; i++) {
    //go for another iteration, if elements of relation are equal
    if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[i].y))) {
      continue;
    }
    int antisym = 1;
    for (unsigned int j = 0; j < relations->arr[arr_position].used; j++) {
      if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[j].y)) &&
          !(strcmp(relations->arr[arr_position].arr[i].y, relations->arr[arr_position].arr[j].x))) {
            antisym = 0;
            break;
      }
    }
    //print false in case of existing symmetry
    if (antisym == 0) {
      printf("false\n");
      return;
    }
  }
  printf("true\n");
}

/**
 * @brief check if relation is transitive
 * @param relations vector of relations
 * @param arr_position index of relation in array
 */
void transitive_rel(vrel_t *relations, int arr_position) {
  for (unsigned int i = 0; i < relations->arr[arr_position].used; i++) {
    for (unsigned int j = 0; j < relations->arr[arr_position].used; j++) {
      //go for another iteration, if elements of relations are equal
      if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[i].y)) ||
          !(strcmp(relations->arr[arr_position].arr[j].x, relations->arr[arr_position].arr[j].y))) {
            break;
      }
      if (!(strcmp(relations->arr[arr_position].arr[i].y, relations->arr[arr_position].arr[j].x))) {
        int trans = 0;
        for (unsigned int z = 0; z < relations->arr[arr_position].used; z++) {
          if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[z].x)) &&
              !(strcmp(relations->arr[arr_position].arr[j].y, relations->arr[arr_position].arr[z].y))) {
                trans = 1;
                break;
              }
        }
        //print false in case of something missing
        if (trans != 1) {
          printf("false\n");
          return;
        }
      }
    }
  }
  printf("true\n");
}

/**
 * @brief check if relation is function
 * @param relations vector of relations
 * @param arr_position index of relation in array
 */
void function_rel(vrel_t *relations, int arr_position) {
  for (unsigned int i = 0; i < relations->arr[arr_position].used; i++) {
    for (unsigned int j = i + 1; j < relations->arr[arr_position].used; j++) {
      if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[j].x))) {
        printf("false\n");
        return;
      }
    }
  }
  printf("true\n");
}
/* ======================================= RELATION OPERATIONS END ======================================= */

int main(int argc, char *argv[]){
  FILE *fp;
  if (!(fp = process_args(argc, argv))){
    fprintf(stderr, "Error: fail during file reading\n"
                    "    program start form:\n"
                    "    ./setcal FILE\n");
    return 1;
  }
  
  vec_t universe;
  vec_constructor(&universe, 0);

  vrel_t relations;
  if (!vrel_constructor(&relations)){
    // TODO handle vector of relations malloc failed
    return 1;
  }
  vvec_t vectors;
  if (!vvec_constructor(&vectors)){
    // TODO handle vector malloc failed
    return 1;
  }
  
  int line_type;
  int vec_count = 0;
  int rel_count = 0;
  for (int i = 0; get_line_type(&line_type, fp); i++){
    switch (line_type){
      case S:
        printf("> switch: new vector\n");
        if (!vec_constructor(&(vectors.arr[vec_count]), i)){
          // TODO handle vector construction failed
          return 1;
        }
        new_vec(&vectors.arr[vec_count], fp);
        vec_count++;
        break;
      case R:
        printf("> switch: new relation\n");
        if (!rel_constructor(&(relations.arr[rel_count]), i)){
          // TODO handle relation malloc failed
          return 1;
        }
        new_rel(&relations.arr[rel_count], fp);
        rel_count++;
        break;
      case C:
        printf("> switch: new command\n");
        // TODO new command
        break;
      case U:
        printf("> switch: new universe\n");
        new_vec(&universe, fp);
        vec_print(&universe, 'U');
        break;
    }
  }
  // TODO handle EOF

  /* ============== EXEMPLE OF RELATION OPERATIONS USAGE (DELETE AFTER PARSING IMPLEMENTATION) ============== */
  /*
  int arr_position;
  if ((arr_position = find_relation(&relations, 2)) != -1) {
    reflexive_rel(&universe, &relations, arr_position);
  } else {
    fprintf(stderr, "Error: there aren't relation(R) on given line");
    return 1;
  }

  if ((arr_position = find_relation(&relations, 3)) != -1) {
    symmetric_rel(&relations, arr_position);
  } else {
    fprintf(stderr, "Error: there aren't relation(R) on given line");
    return 1;
  }

  if ((arr_position = find_relation(&relations, 2)) != -1) {
    antisymmetric_rel(&relations, arr_position);
  } else {
    fprintf(stderr, "Error: there aren't relation(R) on given line");
    return 1;
  }

  if ((arr_position = find_relation(&relations, 3)) != -1) {
    transitive_rel(&relations, arr_position);
  } else {
    fprintf(stderr, "Error: there aren't relation(R) on given line");
    return 1;
  }
  
  if ((arr_position = find_relation(&relations, 3)) != -1) {
    function_rel(&relations, arr_position);
  } else {
    fprintf(stderr, "Error: there aren't relation(R) on given line");
    return 1;
  }
  */
  /* ============== EXEMPLE OF RELATION OPERATIONS USAGE (DELETE AFTER PARSING IMPLEMENTATION) ============== */

  vec_print(&vectors.arr[0], 'S');
  vec_destructor(&vectors.arr[0]);

  rel_print(&relations.arr[0]);
  rel_destructor(&relations.arr[0]);

  vec_destructor(&universe);
  // destruct all vectors in vectors->arr
  for (int i = 0; i < vec_count; i++){
    vec_destructor(&(vectors.arr[i]));
  }
  // destruct all relations in relations->arr
  for (int i = 0; i < rel_count; i++){
    rel_destructor(&(relations.arr[i]));
  }
  vvec_destructor(&vectors);
  vrel_destructor(&relations);
  fclose(fp);
  return 0;
}
