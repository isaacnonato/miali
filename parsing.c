#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpc.h"

#include <editline/readline.h>

static char input[2048]

int main(int argc, char** argv) {
    /* create parsers */
    mpc_parser_t* Number     = mpc_new("number");
    mpc_parser_t* Operator   = mpc_new("operator");
    mpc_parser_t* Expression = mpc_new("expression");
    mpc_parser_t* Maili      = mpc_new("maili");

    /*define them with the following language*/

    mpca_lang(MPCA_LANG_DEFAULT,
    "number    : /-?[0-9]+/                                       ; \
    operator   : '+' | '-' | '*' | '/' | '%%'                      ; \
    expression : <number> | '(' <operator> <expression> ')'       ; \
    maili      : /^/ <operator> <expr>+ /$/                       ; \
    ", Number, Operator, Expression, Maili);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Maili, &r)) {
        /* on success, print the AST */
        mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }

    /* undefine and delete parsers */
    mpc_cleanup(4, Number, Operator, Expression, Maili);
    return 0;
}