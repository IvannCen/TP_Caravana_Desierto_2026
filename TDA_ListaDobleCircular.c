#include "TDA_ListaDobleCircular.h"
#include "tablero.h"

void crearListaDoble(tListaDobleC *pl)
{
    *pl = NULL;
}

int listaVacia(const tListaDobleC *pl)
{
    return *pl == NULL;
}

int ponerAlFinalEnListaCircular(tListaDobleC *pl, const void *dato, unsigned tam)
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

void mostrarListaDeIzqADer(tListaDobleC *pl, void (* mostrar)(const void *a))
{
    tNodo *act;

    if(!(*pl))
        return;



    act = (*pl)->sig;

    if(act)
    {
        do
        {
            mostrar(act->dato);
            act = act->sig;
        }
        while(act != (*pl)->sig);
    }
}


