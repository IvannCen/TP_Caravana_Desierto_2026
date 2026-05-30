#include "TDA_ColaDinamica.h"
#include "configuracion.h"

void crearCola(tCola *c)
{
    c->pri = NULL;
    c->ult = NULL;
}

int colaVacia(const tCola *c)
{
    return c->pri == NULL;
}

int colaLlena(const tCola *c, unsigned tam)
{
    return TODO_OK;
}

int ponerEnCola(tCola *c, const void *dato, unsigned tam)
{
    tNodo *nue;

    if(!(nue = (tNodo*)malloc(sizeof(tNodo))))
        return ERROR_SIN_MEM;

    if(!(nue->dato = malloc(tam)))
    {
        free(nue);
        return ERROR_SIN_MEM;
    }

    memcpy(nue->dato, dato, tam);
    nue->tamDato = tam;
    nue->sig = NULL;

    if(c->ult)
        c->ult->sig = nue;
    else
        c->pri = nue;

    c->ult = nue;

    return TODO_OK;
}

int sacarDeCola(tCola *c, void *dato, unsigned tam)
{
    tNodo *elim = c->pri;

    if(!(elim))
        return COLA_VACIA;

    memcpy(dato, elim->dato, MIN(elim->tamDato, tam));
    c->pri = elim->sig;

    free(elim->dato);
    free(elim);

    if(!(c->pri))
        c->ult = NULL;

    return TODO_OK;
}

int verPrimero(const tCola *c, void *dato, unsigned tam)
{
    if(!(c->pri))
        return COLA_VACIA;

    memcpy(dato, c->pri->dato, MIN(c->pri->tamDato, tam));

    return TODO_OK;
}

void vaciarCola(tCola *c)
{
    tNodo *elim;

    while(c->pri)
    {
        elim = c->pri;
        c->pri = elim->sig;
        free(elim->dato);
        free(elim);
    }
    c->ult = NULL;
}
