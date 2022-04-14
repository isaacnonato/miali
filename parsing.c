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
    mpc_parser_t* Miali      = mpc_new("miali");

    /*define them with the following language*/

    mpca_lang(MPCA_LANG_DEFAULT,
    "number    : /-?[0-9]+/                                       ; \
    operator   : '+' | '-' | '*' | '/' | '%%'                      ; \
    expression : <number> | '(' <operator> <expression> ')'       ; \
    miali      : /^/ <operator> <expr>+ /$/                       ; \
    ", Number, Operator, Expression, Miali);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Miali, &r)) {
        /* on success, print the AST */
        mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }

    /* undefine and delete parsers */
    mpc_cleanup(4, Number, Operator, Expression, Miali);
    return 0;
}