#include "stack.h"
#include "verificator.h"

long long CANARY_DATA_LEFT = 0xDEDDEADAAADDDD;
long long CANARY_DATA_RIGHT = 0xDEADDED;
const int POISON = 1337228;
const int SIZEOFEL = sizeof(DATA);
const int NUMBOFCANARIES = 2;
int INIT_SIZE = 100;

static void ResizeStackUp (Stack *st);

void InitStack (Stack *st) {
    if (st == nullptr) {
        Dump (st, DATA, NULL_PTR_TO_STACK);
    }
    ASSERT_OK (st->capacity > 0 || st->sizeOfStack > 0, st, RECREATING_STACK, return;);

    st->capacity = INIT_SIZE;
    st->sizeOfStack = 0;

    st->data = (DATA*)calloc (INIT_SIZE + NUMBOFCANARIES * sizeof (CANARY_DATA_LEFT), sizeof (DATA));

    void *ptr = (void*) ((char*)st->data + (st->capacity) * sizeof (DATA) + sizeof (CANARY_DATA_LEFT));
    ptr = memcpy (ptr, (const void*) (&CANARY_DATA_RIGHT), sizeof (CANARY_DATA_RIGHT));
    ASSERT_OK (ptr == nullptr, st, ALLOC_FAULT, ;);

    ptr = (void*)st->data;
    ptr = memcpy(ptr, (const void*)(&CANARY_DATA_LEFT), sizeof(CANARY_DATA_LEFT));
    ASSERT_OK(ptr == nullptr, st, ALLOC_FAULT, ;);

    st->data = (DATA*) ((char*)st->data + sizeof(CANARY_DATA_LEFT));

    st->hashStack = MurmurHash2 (st);

    ASSERTION_CHECK(st);
}

void PushStack (Stack *st, DATA value) {

    ASSERTION_CHECK(st);

    if (st->sizeOfStack >= st->capacity) {
        ResizeStackUp (st);
    }

    ASSERTION_CHECK(st);

    void *ptr = memcpy ((void*)(st->data + st->sizeOfStack), (const void*)(&value), SIZEOFEL);
    ASSERT_OK(ptr == nullptr, st, ALLOC_FAULT, ;);

    st->sizeOfStack++;

    st->hashStack = MurmurHash2 (st);

    ASSERTION_CHECK(st);

}


static void ResizeStackUp (Stack *st) {

    ASSERTION_CHECK(st);

    st->capacity = 2 * st->capacity + 1;

    void *Ptr = realloc ((void*)(st->data - sizeof(CANARY_DATA_LEFT) / sizeof (DATA)),
                         st->capacity * sizeof (DATA) + sizeof (CANARY_DATA_LEFT) + sizeof (CANARY_DATA_RIGHT));
    ASSERT_OK(Ptr == nullptr, st, ALLOC_FAULT, ;);
    st->data = (DATA*)Ptr;

    st->data = (DATA*) ((char*)st->data + sizeof (CANARY_DATA_LEFT)) ;

    void *ptr = ((char*)st->data + st->capacity * sizeof (DATA));
    ptr = memcpy (ptr, (const void*) (&CANARY_DATA_RIGHT), sizeof (CANARY_DATA_RIGHT));
    ASSERT_OK(ptr == nullptr, st, ALLOC_FAULT, ;);

    st->hashStack = MurmurHash2 (st);

    ASSERTION_CHECK(st);
}


DATA PopStack (Stack *st) {
    ASSERT_OK(st->sizeOfStack == 0, st, EMPTY_STACK, return POISON; );

    checkIfStackIsValid (st);
    ASSERT_OK(st->err != OK_WITH_STACK, st, st->err, return 0;);

    st->sizeOfStack--;

    DATA val = *(st->data + st->sizeOfStack);
    void *ptr = memcpy ((void*)(st->data + st->sizeOfStack), (const void*)(&POISON), SIZEOFEL);
    ASSERT_OK(ptr == nullptr, st, ALLOC_FAULT, ;);

    st->hashStack = MurmurHash2 (st);

    checkIfStackIsValid (st);
    ASSERT_OK(st->err != OK_WITH_STACK, st, st->err, return 0;);

    return val;
}


DATA TopStack (Stack *st) {
    ASSERT_OK(st->sizeOfStack == 0, st, EMPTY_STACK, return POISON; );

    checkIfStackIsValid (st);
    ASSERT_OK(st->err != OK_WITH_STACK, st, st->err, return 0;);

    DATA val = *(st->data + st->sizeOfStack - 1);
    void *ptr = memcpy ((void*)(st->data + st->sizeOfStack), (const void*)(&POISON), SIZEOFEL);
    ASSERT_OK(ptr == nullptr, st, ALLOC_FAULT, ;);
    st->err = OK_WITH_STACK;

    st->hashStack = MurmurHash2 (st);

    checkIfStackIsValid (st);
    ASSERT_OK(st->err != OK_WITH_STACK, st, st->err, return 0;);
    return val;
}


void DestructStack (Stack *st) {

    ASSERTION_CHECK(st);

    free ((char*)st->data - sizeof (CANARY_DATA_LEFT));

    st->capacity = POISON;
    st->sizeOfStack = POISON;

    st->hashStack = POISON;
    st->err = STACK_DESTRUCTED;
}


void ResizeStackDown (Stack *st) {
    ASSERTION_CHECK(st);

    st->capacity = st->sizeOfStack + 1;

    void *Ptr = realloc ((void*)(st->data - sizeof(CANARY_DATA_LEFT) / sizeof (DATA)), st->capacity * sizeof (DATA)
                          + sizeof (CANARY_DATA_LEFT) + sizeof (CANARY_DATA_RIGHT));
    ASSERT_OK(Ptr == nullptr, st, ALLOC_FAULT, ;);
    st->data = (DATA*)Ptr;

    st->data = (DATA *) ((char*)st->data + sizeof (CANARY_DATA_LEFT));

    void *ptr = ((char*)st->data + st->capacity * sizeof (DATA));
    ptr = memcpy (ptr, (const void*)(&CANARY_DATA_RIGHT), sizeof (CANARY_DATA_RIGHT));
    ASSERT_OK(ptr == nullptr, st, ALLOC_FAULT, ;);

    st->hashStack = MurmurHash2 (st);

    ASSERTION_CHECK(st);
}
