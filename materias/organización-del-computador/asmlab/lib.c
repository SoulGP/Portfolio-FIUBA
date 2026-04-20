#include "lib.h"

funcCmp_t *getCompareFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcCmp_t *)&intCmp;
        break;
    case TypeString:
        return (funcCmp_t *)&strCmp;
        break;
    case TypeCard:
        return (funcCmp_t *)&cardCmp;
        break;
    default:
        break;
    }
    return 0;
}
funcClone_t *getCloneFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcClone_t *)&intClone;
        break;
    case TypeString:
        return (funcClone_t *)&strClone;
        break;
    case TypeCard:
        return (funcClone_t *)&cardClone;
        break;
    default:
        break;
    }
    return 0;
}
funcDelete_t *getDeleteFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcDelete_t *)&intDelete;
        break;
    case TypeString:
        return (funcDelete_t *)&strDelete;
        break;
    case TypeCard:
        return (funcDelete_t *)&cardDelete;
        break;
    default:
        break;
    }
    return 0;
}
funcPrint_t *getPrintFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcPrint_t *)&intPrint;
        break;
    case TypeString:
        return (funcPrint_t *)&strPrint;
        break;
    case TypeCard:
        return (funcPrint_t *)&cardPrint;
        break;
    default:
        break;
    }
    return 0;
}

/** Int **/

int32_t intCmp(int32_t *a, int32_t *b)
{
    if (*a < *b) 
        return 1;
    else if (*a > *b) 
        return -1;
    return 0;
}

void intDelete(int32_t *a)
{
    free(a);
}

void intPrint(int32_t *a, FILE *pFile)
{
    fprintf(pFile, "%d", *a);
}

int32_t *intClone(int32_t *a)
{
    int32_t *clon = malloc(sizeof(int32_t));
    *clon = *a;
    return clon;
}

/** Lista **/

listElem_t *crearElem(void *data, type_t t)
{
    listElem_t *elem = malloc(sizeof(listElem_t));
    elem->next = NULL;
    elem->prev = NULL;
    funcClone_t *funcion = getCloneFunction(t);
    elem->data = funcion(data);
    return elem;
}

void *removerElem(listElem_t *elem, uint8_t *size)
{   
    void *data = elem->data;
    free(elem);
    (*size)--;
    return data;
}

list_t *listNew(type_t t)
{
    list_t *lista = malloc(sizeof(list_t));
    lista->first = NULL;
    lista->last = NULL;
    lista->size = 0;
    lista->type = t;
    return lista;
}

uint8_t listGetSize(list_t *l)
{
    return l->size;
}

void *listGet(list_t *l, uint8_t i)
{
    if (i >= listGetSize(l))
        return 0;

    if (i == 0)
        return l->first->data;
    else if (i == listGetSize(l) -1)
        return l->last->data;

    listElem_t *actual = l->first;
    for (uint8_t j = 0; j < i; j++)
        actual = actual->next;
    return actual->data;
}

void listAddFirst(list_t *l, void *data)
{
    listElem_t *elem = crearElem(data, l->type);

    if (listGetSize(l) == 0) {
        l->first = elem;
        l->last = elem;
    } else {
        elem->next = l->first;
        l->first->prev = elem;
        l->first = elem;
    }
    l->size++;
}

void listAddLast(list_t *l, void *data)
{
    listElem_t *elem = crearElem(data, l->type);

    if (listGetSize(l) == 0) {
        l->first = elem;
        l->last = elem;
    } else {
        elem->prev = l->last;
        l->last->next = elem;
        l->last = elem;
    }
    l->size++;
}

list_t *listClone(list_t *l)
{
    list_t *clon = listNew(l->type);

    listElem_t *actual = l->first;
    while (actual != NULL){
        listAddLast(clon, actual->data);
        actual = actual->next;
    }
    return clon;
}

void *listRemove(list_t *l, uint8_t i)
{
    if (i >= listGetSize(l))
        return 0;

    listElem_t *elem = l->first; 
    if (i == 0) {
        l->first = elem->next;
        if (l->first)
            l->first->prev = NULL;
        else
            l->last = NULL;
        return removerElem(elem, &l->size);
    }
    else if (i == listGetSize(l) -1) {
        elem = l->last;
        l->last = elem->prev;
        if (l->last)
            l->last->next = NULL;
        else
            l->first = NULL;
        return removerElem(elem, &l->size);
    }

    for (uint8_t j = 0; j < i; j++) {
        elem = elem->next;
    }
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
    return removerElem(elem, &l->size);
}

void listSwap(list_t *l, uint8_t i, uint8_t j)
{
    if (i >= listGetSize(l) || j >= listGetSize(l))
        return;

    listElem_t *elemI = l->first;
    listElem_t *elemJ = l->first;

    for (uint8_t k = 0; k < i; k++)
        elemI = elemI->next;
    
    for (uint8_t k = 0; k < j; k++)
        elemJ = elemJ->next;

    void *aux = elemI->data;
    elemI->data = elemJ->data;
    elemJ->data = aux;
}

void listDelete(list_t *l)
{
    funcDelete_t *funcion = getDeleteFunction(l->type);
    listElem_t *actual = l->first;

    while (actual) {
        listElem_t *siguiente = actual->next;
        if (funcion && actual->data)
            funcion(actual->data);
        free(actual);
        actual = siguiente;
    }

    free(l);
}

void listPrint(list_t *l, FILE *pFile)
{
 
    funcPrint_t *funcion = getPrintFunction(l->type);

    fprintf(pFile, "[");
    listElem_t *actual = l->first;

    while (actual != NULL) {
        funcion(actual->data, pFile); 
        actual = actual->next;
        if (actual != NULL)
            fprintf(pFile, ","); 
    }
    fprintf(pFile, "]");
}


/** Game **/

game_t *gameNew(void *cardDeck, funcGet_t *funcGet, funcRemove_t *funcRemove, funcSize_t *funcSize, funcPrint_t *funcPrint, funcDelete_t *funcDelete)
{
    game_t *game = (game_t *)malloc(sizeof(game_t));
    game->cardDeck = cardDeck;
    game->funcGet = funcGet;
    game->funcRemove = funcRemove;
    game->funcSize = funcSize;
    game->funcPrint = funcPrint;
    game->funcDelete = funcDelete;
    return game;
}
int gamePlayStep(game_t *g)
{
    int applied = 0;
    uint8_t i = 0;
    while (applied == 0 && i + 2 < g->funcSize(g->cardDeck))
    {
        card_t *a = g->funcGet(g->cardDeck, i);
        card_t *b = g->funcGet(g->cardDeck, i + 1);
        card_t *c = g->funcGet(g->cardDeck, i + 2);
        if (strCmp(cardGetSuit(a), cardGetSuit(c)) == 0 || intCmp(cardGetNumber(a), cardGetNumber(c)) == 0)
        {
            card_t *removed = g->funcRemove(g->cardDeck, i);
            cardAddStacked(b, removed);
            cardDelete(removed);
            applied = 1;
        }
        i++;
    }
    return applied;
}
uint8_t gameGetCardDeckSize(game_t *g)
{
    return g->funcSize(g->cardDeck);
}
void gameDelete(game_t *g)
{
    g->funcDelete(g->cardDeck);
    free(g);
}
void gamePrint(game_t *g, FILE *pFile)
{
    g->funcPrint(g->cardDeck, pFile);
}
