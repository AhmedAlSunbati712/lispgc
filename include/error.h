#ifndef ERROR_H
#define ERROR_H
#include <setjmp.h>
#include <stdbool.h>

// Where an error jumps back to. The REPL fills it in with setjmp
// before evaluating each expression.
extern jmp_buf error_jump;

// True once the REPL has called setjmp on error_jump
extern bool error_jump_set;

#endif
