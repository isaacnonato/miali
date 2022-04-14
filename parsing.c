#include "mpc.h"
#include <editline/readline.h>

int number_of_nodes(mpc_ast_t* a);
long eval(mpc_ast_t* a);
long eval_operator(long x, char* op, long y);

int main(int argc, char** argv) {
  
  mpc_parser_t* Number         = mpc_new("number");
  mpc_parser_t* Operator       = mpc_new("operator");
  mpc_parser_t* Expression     = mpc_new("expression");
  mpc_parser_t* Miali          = mpc_new("miali");
  
  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
      number   : /-?[0-9]+[.]?([0-9]+)?/ ;                             \
      operator : '+' | '-' |'*' | '%' | '/' | '^' ;                  \
      expression     : <number> | '(' <operator> <expression>+ ')' ;  \
      miali    : /^/ <operator> <expression>+ /$/ ;                    \
    ",
    Number, Operator, Expression, Miali);
  
  puts("Miali Version 0.0.0.2");
  puts("Press Ctrl+c to Exit\n");
  
  while (1) {
  
    char* input = readline("miali> ");
    add_history(input);
    
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Miali, &r)) {
      mpc_ast_print(r.output);
      long result = eval(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    // this is segfaulting
    // /* load ast from output */
    // mpc_ast_t* a = r.output;
    // printf("Tag: %s\n", a->tag);
    // printf("Contents: %s\n", a->contents);
    // printf("Number of children%i\n", a->children_num);

    // /* get first child */
    // mpc_ast_t* c0 = a->children[0];
    // printf("First Child Tag%s\n", c0->tag);
    // printf("First child contents: %s\n", c0->contents);
    // printf("First Child's number of children%i\n", c0->children_num);

    free(input);
  }
  
  mpc_cleanup(4, Number, Operator, Expression, Miali);

  return 0;
}

int number_of_nodes(mpc_ast_t* a) {
  if (a->children_num == 1) { return 1; }
  int total = 1;
  for (int i = 0; i < a->children_num; i++) {
    total = total + number_of_nodes(a->children[i]);
  }
  return total;
}  

long eval(mpc_ast_t* a) {
  /* if tagged as a number, it is returned directly */
  if (strstr(a->tag, "number")) {
    return atoi(a->contents);
  }

  char* operator = a->children[1]->contents; // the operator is always in the 2nd child

  long x = eval(a->children[2]);

  /* iterate the remaining children */
  int i = 3;
  while(strstr(a->children[i]->tag, "expression")) {
    x = eval_operator(x, operator, eval(a->children[i]));
    i++;
  }
  return x;
}

long eval_operator(long x, char* op, long y) {
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  if (strcmp(op, "%") == 0) { return x % y; }
  if (strcmp (op, "^") == 0) { return pow(x, y); }
  return 0;
}