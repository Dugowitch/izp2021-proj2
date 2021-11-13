#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ELEM_LEN 31
enum {ERR, U, S, R, C};

typedef struct {
  char **arr;
  size_t used;
  size_t size;
} vec_t;

typedef struct {
  char *x;
  char *y;
} tuple_t;

typedef struct {
  tuple_t *arr;
  size_t used;
  size_t size;
} rel_t;

/**
 * @brief BASIC VECTOR FUNCTIONS
 */

void vec_print(vec_t *v){
  for(size_t i = 0; i < v->used; i++)
      printf("%s ", v->arr[i]);
}

void vec_init(vec_t *v, size_t size) {
  v->arr = malloc(size * sizeof(char*));//TODO potential error
  v->used = 0;
  v->size = size;
}

void vec_append(vec_t *v, char* element) {
  if (v->used == v->size) {
    v->size += 2;
    v->arr = realloc(v->arr, v->size * sizeof(char*));//TODO potential error
  }
  v->arr[v->used] = (char*) malloc((strlen(element)+1)*sizeof(char));//TODO potential error
  strcpy(v->arr[v->used], element);
  v->used++;
}

void vec_free(vec_t *v) {
  free(v->arr);
  v->arr = NULL;
  v->used = v->size = 0;
}

/**
 * @brief BASIC RELATION & TUPLE FUNCTIONS
 */

void tuple_print(tuple_t t){
  printf("(%s %s) ",t.x, t.y);
}

void rel_print(rel_t *r){
  printf("R ");
  for(size_t i = 0; i < r->used; i++){
    tuple_print(r->arr[i]);
  }
  printf("\n");
}

/**
 * @brief PARSE ARGS and RETURN FILE POINTER
 */

FILE *process_args(int argc, char *argv[]){
  if (argc > 1){
    return fopen(argv[1], "r");
  }
  return NULL;
}

/**
 * @brief read first letter on a line
 */

int get_line_type(FILE *f){
  char c;
  c = fgetc(f);
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
  }
  return ERR;
}

int main(int argc, char *argv[]){
  FILE *fp;
  fp = process_args(argc, argv);
  if (fp != NULL){
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
  }
  else {
    // TODO throw err - could not load file
  }
  fclose(fp);  

  // vec_t v;
  // vec_init(&v, 3);
  // vec_append(&v, "bumbum");
  // vec_append(&v, "do");
  // vec_append(&v, "poop poop");
  // vec_print(&v);
  // vec_free(&v);

  // tuple_t t;
  // t.x = "abc";
  // t.y = "def";
  // tuple_print(t);

  return 0;
}