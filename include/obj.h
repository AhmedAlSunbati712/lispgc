#ifndef OBJECT_H
#define OBJECT_H
#include <stdbool.h>
typedef enum {
    T_FREE,
    T_NIL,
    T_TRUE,
    T_INT,
    T_SYMBOL,
    T_CONS,
    T_PRIM,
    T_CLOSURE
} Tag;

typedef struct Object Object;
struct Object{
    Tag tag;
    bool marked;
    union {
        struct {Object *car;  Object *cdr;} cons; // For T_CONS
        long value; // For T_INT
        char *name; // T_SYMBOL
        Object *next; // T_FREE
        struct {Object *params; Object *body; Object *env;} closure; // T_CLOSURE
    };
};

extern Object nil_obj;
extern Object true_obj;

#define NIL (&nil_obj)
#define TRUE (&true_obj)
#endif
