#include "verificator.h"
#include "stack.h"

#ifdef Int_type
    void PrintElem (DATA value) {
        printf ("%d\n", value);
    }
    void FPrintElem (FILE *file, DATA value) {
        fprintf (file, "%d\n", value);
    }
#endif // Int_type

#ifdef Double_type
    void PrintElem (DATA value) {
        printf ("%lg\n", value);
    }
    void FPrintElem (FILE *file, DATA value) {
        fprintf (file, "%lg\n", value);
    }
#endif // Double_type

#ifdef Char_type
    void PrintElem (DATA value) {
        printf ("%c\n", value);
    }
    void FPrintElem (FILE *file, DATA value) {
        fprintf (file, "%c\n", value);
    }
#endif // Char_type

void checkIfStackIsValid (Stack *st) {
    int err = OK_WITH_STACK;

    while (1) {

        if (st == nullptr) {
            err = NULL_PTR_TO_STACK;
            break;
        }

        else if (st->data == nullptr) {
            err = NULL_PTR_TO_STACK_DATA;
            break;
        }

        else if (st->sizeOfStack < 0) {
            err = STACK_SIZE_WRONG;
            break;
        }

        else if (st->capacity <= 0) {
            err = STACK_CAPACITY_WRONG;
            break;
        }

        else if (st->capacity == POISON && st->sizeOfStack == POISON && st->err == STACK_DESTRUCTED) {
            err = STACK_DESTRUCTED;
            break;
        }

        else if (*(long*)(st->data - sizeof (CANARY_DATA_LEFT) / sizeof (DATA)) != CANARY_DATA_LEFT) {
            err = CANARY_DATA_LEFT_DAMAGED;
            break;
        }

        else if (*(long*)(st->data + st->capacity) != CANARY_DATA_RIGHT) {
            err = CANARY_DATA_LEFT_DAMAGED;
            break;
        }

        if (st->hashStack != MurmurHash2 (st)) {
            st->err = HASH_DAMAGED;
            return;
        }

        else {
            err = OK_WITH_STACK;
            break;
        }
    }

    if (err != OK_WITH_STACK) {
        #ifdef DEBUG_MODE
            Dump(st,DATA, err);
        #endif // DEBUG_MODE

        #ifndef DEBUG_MODE
            if (err == NULL_PTR_TO_STACK) exit (0);
            st->err = err;
        #endif
    } else {
        st->err = OK_WITH_STACK;
    }

}

#define COLOR  "\033[31m"

void PrintError (int error) {
    switch (error) {
        case OK_WITH_STACK: {
            printf ("Stack is OK!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case CANARY_DATA_LEFT_DAMAGED: {
            printf ("Left canary of data is damaged!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case CANARY_DATA_RIGHT_DAMAGED: {
            printf ("Right canary of data is damaged!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case NULL_PTR_TO_STACK_DATA: {
            printf ("Pointer to the stack data is null!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case STACK_SIZE_WRONG: {
            printf ("Stack size is wrong!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case RECREATING_STACK: {
            printf ("Stack has already been created!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case HASH_DAMAGED: {
            printf ("Hash is damaged!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case EMPTY_STACK: {
            printf ("Stack is empty: nothing to pop!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case ALLOC_FAULT: {
            printf ("Allocation failed!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case NULL_PTR_TO_STACK: {
            printf ("Pointer to the stack is null!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case STACK_CAPACITY_WRONG: {
            printf ("Stack capacity is wrong!\n\033[31mError code: %d\033[0m\n\n", error);
            break;
        }
        case STACK_DESTRUCTED: {
            printf ("Stack has already been destructed!\n\033[31mError code: %d\033[0m\n", error);
            break;
        }
        default: {
            printf ("Unknown error occurred!\n" COLOR "Error code: %d" COLOR "\n\n", error);
        }
    }
}

void FPrintError (FILE *file, int error) {
    switch (error) {
        case OK_WITH_STACK: {
            fprintf (file, "Stack is OK!\nError code: %d\n\n", error);
            break;
        }
        case CANARY_DATA_LEFT_DAMAGED: {
            fprintf (file, "Left canary of data is damaged!\nError code: %d\n\n", error);
            break;
        }
        case CANARY_DATA_RIGHT_DAMAGED: {
            fprintf (file, "Right canary of data is damaged!\nError code: %d\n\n", error);
            break;
        }
        case NULL_PTR_TO_STACK_DATA: {
            fprintf (file, "Pointer to the stack data is null!\nError code: %d\n\n", error);
            break;
        }
        case STACK_SIZE_WRONG: {
            fprintf (file, "Stack size is wrong!\nError code: %d\n\n", error);
            break;
        }
        case RECREATING_STACK: {
            fprintf (file, "Stack has already been created!\nError code: %d\n\n", error);
            break;
        }
        case HASH_DAMAGED: {
            fprintf (file, "Hash is damaged!\nError code: %d\n\n", error);
            break;
        }
        case EMPTY_STACK: {
            fprintf (file, "Stack is empty: nothing to pop!\nError code: %d\n\n", error);
            break;
        }
        case ALLOC_FAULT: {
            fprintf (file, "Allocation failed!\nError code: %d\n\n", error);
            break;
        }
        case NULL_PTR_TO_STACK: {
            fprintf (file, "Pointer to the stack is null!\nError code: %d\n\n", error);
            break;
        }
        case STACK_CAPACITY_WRONG: {
            fprintf (file, "Stack capacity is wrong!\nError code: %d\n\n", error);
            break;
        }
        case STACK_DESTRUCTED: {
            fprintf (file, "Stack has already been destructed!\nError code: %d\n", error);
            break;
        }
        default: {
            fprintf (file, "Unknown error occurred!\nError code: %d\n\n", error);
        }
    }
}

void DumpStack (Stack *st, FILE *dump) {

    fprintf (dump,
             "    size = %d\n"
             "    capacity = %d\n"
             "    data[%p]\n"
             "    hash = 0x%x\n", st->sizeOfStack, st->capacity, (void *)st->data, st->hashStack);

    if (st->data == nullptr) return;

    for (int i = 0; i < st->sizeOfStack; i++) {
        fprintf (dump,
                 "      *[%d] = ", i);
        FPrintElem (dump, *(st->data + i));
    }

    fprintf (dump, "Dump succeed!\n\n");
}

void PrintStack (Stack *st) {
    printf ("    size = %d\n"
            "    capacity = %d\n"
            "    data[%p]\n"
            "    hash = 0x%x\n", st->sizeOfStack, st->capacity, (void *)st->data, st->hashStack);


    for (int i = 0; i < st->sizeOfStack; i++) {
        printf ("      *[%d] = ", i);
        PrintElem (*(st->data + i));
    }
}



unsigned int MurmurHash2 (Stack *st)
{

    long long myHash = 0x1337228;

    char *ptr = (char*)st;

    for (size_t i = 0; i < sizeof (*st); i++) {
        myHash ^= (*ptr + i) >> 3;
    }

    if (st->data != nullptr) {
        myHash ^= *(long*)((char*)st->data - sizeof (CANARY_DATA_LEFT)) >> 4;
        myHash ^= *(long*)((char*)st->data + st->capacity * sizeof (DATA)) >> 6;

        for (int i = 0; i < st->sizeOfStack; i++) {
            myHash ^= *(int*)(st->data + i) << 7;
        }
    }


    myHash ^= (myHash >> 6);
    myHash *= 0x228AD;

    return myHash % (16 * 16 * 16 * 16 * 16 * 16);
}
