#include "TDA_ListaDobleCircular.h"

void crearListaDoble(tLista *pl)
{
    *pl = NULL;
}

int listaVacia(const tLista *pl)
{
    return *pl == NULL;
}

int ponerAlFinalEnListaCircular(tLista *pl, const void *dato, unsigned tam)
{
    tNodo *nue, *pri;

    nue = (tNodo *)malloc(sizeof(tNodo));
    if(!nue)
        return 0;

    nue->dato = malloc(tam);
    if(!(nue->dato))
    {
        free(nue);
        return 0;
    }

    memcpy(nue->dato, dato, tam);
    nue->tamDato = tam;

    if(!(*pl))
    {
        nue->sig = nue;
        nue->ant = nue;
    }
    else
    {
        pri = (*pl)->sig;
        nue->ant = *pl;
        nue->sig = pri;
        (*pl)->sig = nue;
        pri->ant = nue;
    }

    *pl = nue;

    return 1;
}
