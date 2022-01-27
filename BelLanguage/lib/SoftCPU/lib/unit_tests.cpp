#include "stack.h"
#include "verificator.h"
#include "unit_tests.h"

void PopEmptyStack (Stack *st) {
    for (int i = 0; i < 150; i++) PushStack (st, 75);
    for (int i = 0; i < 151; i++) PopStack (st);
}

void WrongCapacity (Stack *st) {
    st->capacity = -5;
    PushStack (st, 67);
}

void WrongSize (Stack *st) {
    st->sizeOfStack = -34;
    PushStack (st, 67);
}

void HashDamaged (Stack *st) {
    for (int i = 0; i < 150; i++) PushStack (st, 75);

    st->data[st->sizeOfStack / 2] = 56;

    PushStack (st, 45);
}


