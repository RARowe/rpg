#include "types.h"

void state_stack_init(state_stack_t* s) {
    s->size = 0;
}

int state_stack_peek(state_stack_t* s) {
    if (s->size > 0) {
        return s->stack[s->size - 1];
    } else {
        return -1;
    }
}

int state_stack_push(state_stack_t* s, int e) {
    if (s->size < 10) {
        s->stack[s->size] = e;
        s->size += 1;

        return 1;
    } else {
        return 0;
    }
}

int state_stack_pop(state_stack_t* s) {
    if (s->size > 0) {
        s->size -= 1;

        return s->stack[s->size];
    } else {
        return -1;
    }
}

