#include "TDA_ListaSimple.h"

void crearLista(tListaSimple *l)
{
    *l = NULL;
}

int listaVacia(const tListaSimple *l)
{
    return *l == NULL;
}

int listaLlena(const tListaSimple *l,const void *dato, unsigned tam)
{
    return TODO_OK;
}

int sacarDeListaAlPrincipio(tListaSimple *l, void *dato, unsigned tam)
{
    tNodoListaS *elim = *l;

    if(elim == NULL)
    {
        return LISTA_VACIA;
    }

    *l = elim->sig;
    memcpy(dato, elim->dato, MINIMO(elim->tamDato, tam));
    free(elim->dato);
    free(elim);

    return TODO_OK;
}

int verPrimeroEnLista(const tListaSimple *l, void *dato, unsigned tam)
{
    if(*l == NULL)
        return LISTA_VACIA;

    memcpy(dato, (*l)->dato, MINIMO((*l)->tamDato, tam));

    return TODO_OK;
}

void vaciarLista(tListaSimple *l)
{
    while(*l != NULL)
    {
        tNodoListaS *elim = *l;
        *l = elim->sig;
        free(elim->dato);
        free(elim);
    }
}

int ponerEnListaAlFinal(tListaSimple *l, const void *dato, unsigned tam)
{
    tNodoListaS *nue;

    if(!(nue = (tNodoListaS *)malloc(sizeof(tNodoListaS))) || !(nue->dato = malloc(tam)))
    {
        free(nue);
        return ERROR;
    }

    memcpy(nue->dato, dato, tam);
    nue->tamDato = tam;
    nue->sig = NULL;

    while(*l)
    {
        l = &(*l)->sig;
    }

    *l = nue;

    return TODO_OK;
}
