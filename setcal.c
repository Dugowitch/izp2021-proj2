#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALLOC_CONST 10
#define MAX_CHAR_SIZE 31

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
  v->arr = malloc(ALLOC_CONST * sizeof(char) * MAX_CHAR_SIZE);
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
    v->arr = realloc(v->arr, (v->size + ALLOC_CONST) * sizeof(char) * MAX_CHAR_SIZE);
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
  r->arr = malloc(ALLOC_CONST * 2 * sizeof(char) * MAX_CHAR_SIZE);
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
    r->arr = realloc(r->arr, (r->size + ALLOC_CONST) * 2 * sizeof(char) * MAX_CHAR_SIZE);
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

// void tuple_print(tuple_t t){
//   printf("(%s %s) ",t.x, t.y);
// }

// void rel_print(rel_t *r){
//   printf("R ");
//   for(unsigned int i = 0; i < r->used; i++){
//     tuple_print(r->arr[i]);
//   }
//   printf("\n");
// }

int main(int argc, char *argv[]){
  (void) argc;
  (void) argv;

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