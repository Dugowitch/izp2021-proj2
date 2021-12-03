// IZP 2021/2020
// Projekt 2 - Práce s datovými strukturami

// Lukáš Matuška (xmatus38)
// Jakub Dugovič (xdugov00)
// Verevkin Aleksandr (xverev00)
// Martin Hrdlička (xhrdli15)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ELEM_LEN 31
#define ALLOC_CONST 10
enum{
  ERR,
  U,
  S,
  R,
  C
};
enum{
  SUBSET,
  SUBSETEQ,
  EQUALS
}; // subset_equal function modes

/**
 * @brief vector structure
 */
typedef struct{
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
  for (unsigned int i = 0; i < v->used; i++)
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
int vec_append(vec_t *v, char *elem){
  if (v->used == v->size){
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
typedef struct{
  char *x;
  char *y;
} tuple_t;

/**
 * @brief relation structure
 */
typedef struct{
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
  for (unsigned int i = 0; i < r->used; i++)
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
int rel_append(rel_t *r, char *elem1, char *elem2){
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
typedef struct{
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
}

/**
 * @brief vector of relations
 */
typedef struct{
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
}

/**
 * @brief parse arguments and return file pointer
 * @param argc count of arguments
 * @param argv arguments
 * @return pointer on file in case of right amount of arguments, else NULL
 */
FILE *process_args(int argc, char *argv[]){
  // check if amount of arguments != 2
  if (argc != 2){
    return NULL;
  }
  else{
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
    }
    else if (iscntrl(c)){
      get_line_type(var, f);
    }
    else{
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
    char *s = malloc(MAX_ELEM_LEN * sizeof(char)); // FIXME free somewhere or strcpy???
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
    char *s1 = malloc(MAX_ELEM_LEN * sizeof(char)); // FIXME free somewhere or strcpy???
    char *s2 = malloc(MAX_ELEM_LEN * sizeof(char)); // FIXME free somewhere or strcpy???
    fscanf(f, "(%s %s", s1, s2);
    s2[strlen(s2) - 1] = '\0'; // removes unwanted bracket
    rel_append(r, s1, s2);
  }
  return 0;
}

/* ======================================= SET OPERATIONS START ======================================= */
/**
 * @brief finds index of vector in vecotrs.arr
 * @param v1 number of line of the first set
 * @param v2 number of line of the second set
 */
int get_param_idx(vvec_t *vectors, unsigned int v1, unsigned int v2, unsigned int *v1_idx, unsigned int *v2_idx){
  for (unsigned int i = 0; i <= vectors->used; i++){
    if (vectors->arr[i].line == v1 - 1){
      *v1_idx = i;
    }
    if (vectors->arr[i].line == v2 - 1){
      *v2_idx = i;
    }
  }
  if ((int)(*v1_idx) == -1 || (int)(*v2_idx) == -1){
    return 1;
  }
  return 0;
}

/**
 * @brief the set on the line v1 minus the set on the line v2
 * @param v1 number of line of the first set
 * @param v2 number of line of the second set
 */
int minus(vvec_t *vectors, unsigned int v1, unsigned int v2){
  vec_t result;
  vec_constructor(&result, -1);
  unsigned int v1_idx = -1;
  unsigned int v2_idx = -1;
  if (get_param_idx(vectors, v1, v2, &v1_idx, &v2_idx)){
    return 1;
  }
  for (unsigned int j = 0; j < vectors->arr[v1_idx].used; j++){
    int same = 0;
    for (unsigned int k = 0; k < vectors->arr[v2_idx].used; k++){
      if (!strcmp(vectors->arr[v1_idx].arr[j], vectors->arr[v2_idx].arr[k])){
        same = 1;
      }
    }
    if (!same){
      vec_append(&result, vectors->arr[v1_idx].arr[j]);
    }
  }
  vec_print(&result, 'S');
  vec_destructor(&result);
  return 0;
}

/**
 * @brief prints true or false if the set on the line v1 MODE the set on the line v2
 * @param v1 number of line of the first set
 * @param v2 number of line of the second set
 * @param mode subseteq/subset/equals
 */
int subset_equal(vvec_t *vectors, unsigned int v1, unsigned int v2, int mode){
  unsigned int v1_idx = -1;
  unsigned int v2_idx = -1;
  if (get_param_idx(vectors, v1, v2, &v1_idx, &v2_idx) == 1){
    return 1;
  }
  int result = 1;
  unsigned int same_counter = 0;
  for (unsigned int i = 0; i < vectors->arr[v1_idx].used; i++){
    int same = 0;
    for (unsigned int j = 0; j < vectors->arr[v2_idx].used; j++){
      if (!strcmp(vectors->arr[v1_idx].arr[i], vectors->arr[v2_idx].arr[j])){
        same = 1;
        same_counter++;
      }
    }
    if (!same){
      result = 0;
      break;
    }
  }
  if (!result)
    printf("false\n");
  else if (mode == SUBSETEQ)
    printf("true\n");
  else if (mode == SUBSET){
    if (same_counter >= vectors->arr[v2_idx].used)
      printf("false\n");
    else
      printf("true\n");
  }
  else{ // mode == EQUALS
    if (same_counter != vectors->arr[v2_idx].used)
      printf("false\n");
    else
      printf("true\n");
  }
  return 0;
}

/* ======================================= SET OPERATIONS END ======================================= */

/* ======================================= RELATION OPERATIONS START ======================================= */
/**
 * @brief find if relation on given line exist
 * @param relations relation to find
 * @param line line of relation
 * @return array index of relation in case of success, else -1
 */
int find_relation(vrel_t *relations, unsigned int line){
  for (unsigned int i = 0; i <= 10; i++){ // TODO cycle until what?
    if (relations->arr[i].line == (line - 1)){
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
void reflexive_rel(vec_t *universe, vrel_t *relations, int arr_position){
  // check if duplicate relation of every element in universe exist
  for (unsigned int i = 0; i < universe->used; i++){
    int ref = 0;
    for (unsigned int j = 0; j < relations->arr[arr_position].used; j++){
      if (!(strcmp(universe->arr[i], relations->arr[arr_position].arr[j].x)) &&
          !(strcmp(universe->arr[i], relations->arr[arr_position].arr[j].y))){
        ref = 1;
        break;
      }
    }
    // print false in case of missing something
    if (ref != 1){
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
void symmetric_rel(vrel_t *relations, int arr_position){
  for (unsigned int i = 0; i < relations->arr[arr_position].used; i++){
    // go for another iteration, if elements of relation are equal
    if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[i].y))){
      continue;
    }
    int sym = 0;
    for (unsigned int j = 0; j < relations->arr[arr_position].used; j++){
      if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[j].y)) &&
          !(strcmp(relations->arr[arr_position].arr[i].y, relations->arr[arr_position].arr[j].x))){
        sym = 1;
        break;
      }
    }
    // print false in case of missing something
    if (sym != 1){
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
void antisymmetric_rel(vrel_t *relations, int arr_position){
  for (unsigned int i = 0; i < relations->arr[arr_position].used; i++){
    // go for another iteration, if elements of relation are equal
    if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[i].y))){
      continue;
    }
    int antisym = 1;
    for (unsigned int j = 0; j < relations->arr[arr_position].used; j++){
      if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[j].y)) &&
          !(strcmp(relations->arr[arr_position].arr[i].y, relations->arr[arr_position].arr[j].x))){
        antisym = 0;
        break;
      }
    }
    // print false in case of existing symmetry
    if (antisym == 0){
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
void transitive_rel(vrel_t *relations, int arr_position){
  for (unsigned int i = 0; i < relations->arr[arr_position].used; i++){
    for (unsigned int j = 0; j < relations->arr[arr_position].used; j++){
      // go for another iteration, if elements of relations are equal
      if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[i].y)) ||
          !(strcmp(relations->arr[arr_position].arr[j].x, relations->arr[arr_position].arr[j].y))){
        break;
      }
      if (!(strcmp(relations->arr[arr_position].arr[i].y, relations->arr[arr_position].arr[j].x))){
        int trans = 0;
        for (unsigned int z = 0; z < relations->arr[arr_position].used; z++){
          if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[z].x)) &&
              !(strcmp(relations->arr[arr_position].arr[j].y, relations->arr[arr_position].arr[z].y))){
            trans = 1;
            break;
          }
        }
        // print false in case of something missing
        if (trans != 1){
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
void function_rel(vrel_t *relations, int arr_position){
  for (unsigned int i = 0; i < relations->arr[arr_position].used; i++){
    for (unsigned int j = i + 1; j < relations->arr[arr_position].used; j++){
      if (!(strcmp(relations->arr[arr_position].arr[i].x, relations->arr[arr_position].arr[j].x))){
        printf("false\n");
        return;
      }
    }
  }
  printf("true\n");
}
/* ======================================= RELATION OPERATIONS END ======================================= */

/**
 * @brief get command and params
 * @param command pointer to string - command name
 * @param params pointer to int arr - param(s) of command
 */
int get_command(FILE *f, char *command, unsigned int *params){
  fscanf(f, "%s", command);
  int counter = 0;
  char temp;

  /** 
   * FIXME double and triple digit numbers
   * maybe use isdigit() from ctype library
  */

  while (((temp = fgetc(f))) != EOF){
    if (iscntrl(temp)){
      break;
    }
    if (temp == ' ')
      continue;
    if ('0' <= temp && temp <= '9'){
      params[counter] = temp - '0';
      counter++;
      continue;
    }
    return 1;
  }
  return 0;
}

/**
 * @brief call command with appropriate params
 * @param command name of the command to call
 * @param params pointer to int arr - param(s) of command
 * @param universe
 * @param vectors vector of all vectors
 * @param relations vector of all relations
 * @return 0 in case of success, 1 otherwise
 */
int call_command(char *command, unsigned int *params, vec_t *universe, vvec_t *vectors, vrel_t *relations){
  // printf("> call command started\n");

  if (!strcmp(command, "empty")){
    // empty A - tiskne true nebo false podle toho, jestli je množina definovaná na řádku A prázdná nebo neprázdná.
  }
  else if (!strcmp(command, "card")){
    // card A - tiskne počet prvků v množině A (definované na řádku A).
  }
  else if (!strcmp(command, "complement")){
    // complement A - tiskne doplněk množiny A.
  }
  else if (!strcmp(command, "union")){
    // union A B - tiskne sjednocení množin A a B.
  }
  else if (!strcmp(command, "intersect")){
    // intersect A B - tiskne průnik množin A a B.
  }
  else if (!strcmp(command, "minus")){
    // minus A B - tiskne rozdíl množin A \ B.
    if (minus(vectors, params[0], params[1])){
      fprintf(stderr, "Error: command minus failed to execute\n");
      return 1;
    }
  }
  else if (!strcmp(command, "subseteq")){
    // subseteq A B - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B.
    if (subset_equal(vectors, params[0], params[1], SUBSETEQ)){
      fprintf(stderr, "Error: command subseteq failed to execute\n");
      return 1;
    }
  }
  else if (!strcmp(command, "subset")){
    // subset A B - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B.
    if (subset_equal(vectors, params[0], params[1], SUBSET)){
      fprintf(stderr, "Error: command subseteq failed to execute\n");
      return 1;
    }
  }
  else if (!strcmp(command, "equals")){
    // equals A B - tiskne true nebo false, jestli jsou množiny rovny.
    if (subset_equal(vectors, params[0], params[1], EQUALS)){
      fprintf(stderr, "Error: command subseteq failed to execute\n");
      return 1;
    }
  }
  else if (!strcmp(command, "reflexive")){
    // reflexive R - tiskne true nebo false, jestli je relace reflexivní.
    int arr_position;
    if ((arr_position = find_relation(relations, params[0])) != -1){
      reflexive_rel(universe, relations, arr_position);
    }
    else{
      fprintf(stderr, "Error: there aren't relation(R) on given line (line %d)\n", params[0]);
      return 1;
    }
  }
  else if (!strcmp(command, "symmetric")){
    // symmetric R - tiskne true nebo false, jestli je relace symetrická.
    int arr_position;
    if ((arr_position = find_relation(relations, params[0])) != -1){
      symmetric_rel(relations, arr_position);
    }
    else{
      fprintf(stderr, "Error: there aren't relation(R) on given line (line %d)\n", params[0]);
      return 1;
    }
  }
  else if (!strcmp(command, "antisymmetric")){
    // antisymmetric R - tiskne true nebo false, jestli je relace antisymetrická.
    int arr_position;
    if ((arr_position = find_relation(relations, params[0])) != -1){
      antisymmetric_rel(relations, arr_position);
    }
    else{
      fprintf(stderr, "Error: there aren't relation(R) on given line (line %d)\n", params[0]);
      return 1;
    }
  }
  else if (!strcmp(command, "transitive")){
    // transitive R - tiskne true nebo false, jestli je relace tranzitivní.
    int arr_position;
    if ((arr_position = find_relation(relations, params[0])) != -1){
      transitive_rel(relations, arr_position);
    }
    else{
      fprintf(stderr, "Error: there aren't relation(R) on given line (line %d)\n", params[0]);
      return 1;
    }
  }
  else if (!strcmp(command, "function")){
    // function R - tiskne true nebo false, jestli je relace R funkcí.
    int arr_position;
    if ((arr_position = find_relation(relations, params[0])) != -1){
      function_rel(relations, arr_position);
    }
    else{
      fprintf(stderr, "Error: there aren't relation(R) on given line (line %d)\n", params[0]);
      return 1;
    }
  }
  else if (!strcmp(command, "domain")){
    // domain R - tiskne definiční obor funkce R (lze aplikovat i na relace - první prvky dvojic).
  }
  else if (!strcmp(command, "codomain")){
    // codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic).
  }
  else if (!strcmp(command, "injective")){
    // injective R - tiskne true nebo false, jestli je funkce R injektivní.
  }
  else if (!strcmp(command, "surjective")){
    // surjective R - tiskne true nebo false, jestli je funkce R surjektivní.
  }
  else if (!strcmp(command, "bijective")){
    // bijective R - tiskne true nebo false, jestli je funkce R bijektivní.
  }
  else{
    // bonus commands are not implemented
  }
  return 0;
}

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
      vectors.used++;
      vec_count++;
      break;
    case R:
      printf("> switch: new relation\n");
      if (!rel_constructor(&(relations.arr[rel_count]), i)){
        // TODO handle relation malloc failed
        return 1;
      }
      new_rel(&relations.arr[rel_count], fp);
      relations.used++;
      rel_count++;
      break;
    case C:;
      char command[MAX_ELEM_LEN];
      unsigned int params[2] = {-1, -1};
      printf("> switch: new command\n");
      int successful = get_command(fp, &command[0], &params[0]);
      if (successful){
        // TODO error - handle improper command formatting
      }
      if (call_command(&command[0], &params[0], &universe, &vectors, &relations)){
        return 1;
      }
      break;
    case U:
      printf("> switch: new universe\n");
      new_vec(&universe, fp);
      vec_print(&universe, 'U');
      break;
    }
  }
  // TODO handle EOF

/**
 * FIXME free all vec_t's in vectors
 * FIXME free all rel_t's in relations
*/

  vvec_destructor(&vectors);
  vrel_destructor(&relations);
  fclose(fp);
  return 0;
}
