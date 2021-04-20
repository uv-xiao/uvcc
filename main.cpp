#include "ast.h"
#include <iostream>
#include <cstdio>
#include <fstream>

extern NRoot *root;
extern int yyparse();
extern int yylex_destroy();
extern void yyset_lineno(int _line_number);

int main(int argc, char **argv) {
#ifndef DEBUG
    freopen(argv[3], "r", stdin);
    freopen(argv[5], "w", stdout);
#endif
    yyset_lineno(1);
#ifdef DEBUG
    printf("Start parsing\n");
#endif
    yyparse();
#ifdef DEBUG
    printf("Finish parsing\n");
#endif
    yylex_destroy();

    root->generate();
    return 0;
}
