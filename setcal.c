#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char **arr;
  size_t used;
  size_t size;
} vec_t;

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

int main(int argc, char *argv[]){
    (void) argc;
    (void) argv;

    // vec_t v;
    // vec_init(&v, 3);
    // vec_append(&v, "bumbum");
    // vec_append(&v, "do");
    // vec_append(&v, "poop poop");
    // vec_print(&v);
    // vec_free(&v);

    return 0;
}