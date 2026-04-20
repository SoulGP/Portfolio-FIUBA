#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "lib.h"

void testArrayAddLast(void) {
    array_t array;
    array.type = TypeInt;
    array.size = 0;
    array.capacity = 3;
    array.data = malloc(sizeof(void*) * array.capacity);
    assert(array.data != NULL);

    int valores[3] = { 42, 100, -7 };
    for (int i = 0; i < 3; i++) {
        arrayAddLast(&array, &valores[i]);
        assert(array.size == i + 1);
        assert(array.data[i] != &valores[i]);
        assert(*(int*)array.data[i] == valores[i]);
    }
    
    for (int i = 0; i < array.size; i++) {
        free(array.data[i]);
    }
    free(array.data);

    printf("testArrayAddLast: Todo correcto.\n");
}

void testArrayNew(void) {
    array_t* array = arrayNew(TypeInt, 5);
    assert(array != NULL);
    assert(array->type == TypeInt);
    assert(array->size == 0);
    assert(array->capacity == 5);
    assert(array->data != NULL);

    for (int i = 0; i < array->capacity; i++) {
        free(array->data[i]);
    }
    free(array->data);
    free(array);

    printf("testArrayNew: Todo correcto.\n");
}

void testStrClone(void) {
    char original[] = "Soy una prueba";
    char* stringClonado = strClone(original);

    assert(stringClonado != NULL);
    assert(strcmp(original, stringClonado) == 0);
    strDelete(stringClonado);
    printf("testStrClone: Todo correcto.\n");
}

void testArrayDelete(void){
    array_t *array = arrayNew(TypeInt, 5);

    for (int i = 0; i < 3; i++) {
        int* valor = malloc(sizeof(int));
        *valor = i * 10;
        arrayAddLast(array, valor);
        free(valor);
    }
    for (int i = 0; i < array->size; i++) {
        printf("array->data[%d] = %p\n", i, (void*)array->data[i]);
    }
    arrayDelete(array);
}
void testCardClone(char *testType, int testNum)
{
    int32_t number = 12;
    char str[] = "espada";
    card_t *card = cardNew(str, &number);
    card_t *cardClonada = cardClone(card);

    assert(strcmp(card->suit, cardClonada->suit) == 0);
    assert(*(card->number) == *(cardClonada->number));
    assert(card->stacked->size == cardClonada->stacked->size);
    
    cardDelete(card);
    cardDelete(cardClonada);
    
    printf("cardClone: Todo correcto.\n");

}

void test02ArrayAddLastConUnElemento(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 10);
    int num = 3;
    arrayAddLast(a, &num);
    assert(*(int *)(a->data[0]) == 3);
    assert(a->size == 1);
    arrayDelete(a);
}

int main (void){  
    testStrClone();
    testArrayAddLast();
    testArrayNew();
    testArrayDelete();
    testCardClone("cardClone", 1);
    test02ArrayAddLastConUnElemento("test02ArrayAddLastConUnElemento", 1);
    printf("%zu\n", sizeof(array_t));
    return 0;
}


