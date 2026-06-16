#include "TDA_ListaDobleCircular.h"

void crearListaDoble(tListaDobleC *pl)
{
    *pl = NULL;
}

int listaDobleVacia(const tListaDobleC *pl)
{
    return *pl == NULL;
}

int ponerAlFinalEnListaCircular(tListaDobleC *pl, const void *dato, unsigned tam)
{
    tNodoLista *nue, *pri;

    nue = (tNodoLista *)malloc(sizeof(tNodoLista));
    if(!nue)
        return ERROR;

    nue->dato = malloc(tam);
    if(!(nue->dato))
    {
        free(nue);
        return ERROR;
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

    return TODO_OK;
}

void mostrarListaDeIzqADer(tListaDobleC *pl, void (* mostrar)(const void *a))
{
    tNodoLista *act;

    if(!(*pl))
        return;
    act = (*pl)->sig;

    if(act)
    {
        do
        {
            mostrar(act->dato);
            act = act->sig;
        }while(act != (*pl)->sig);
    }
}

int buscarEnListaCircular(tListaDobleC *pl, void *dato, unsigned tam, int (*cmp)(const void*, const void*))
{
    tNodoLista *act;
    if(!(*pl))
        return ERROR;

    act = (*pl)->sig;
    do {
        if(cmp(act->dato, dato) == 0) {
            memcpy(dato, act->dato, MINI(tam, act->tamDato));
            return TODO_OK;
        }
        act = act->sig;
    } while(act != (*pl)->sig);

    return ERROR;
}

int actualizarEnListaCircular(tListaDobleC *pl, const void *dato, unsigned tam, int (*cmp)(const void*, const void*))
{
    tNodoLista *act;

    if(!(*pl))
        return ERROR;

    act = (*pl)->sig;

    do
    {
        if(cmp(act->dato, dato) == 0)
        {
            memcpy(act->dato, dato, MINI(tam, act->tamDato));
            return TODO_OK;
        }

        act = act->sig;

    } while(act != (*pl)->sig);

    return ERROR;
}

int obtenerElementoDesplazado(tListaDobleC *pl, const void *datoOrigen, int pasos, int direccion, void *datoDestino, unsigned tam, int (*cmp)(const void*, const void*))
{
    tNodoLista *act;
    int encontrado = 0;
    int i;

    if(!(*pl))
        return ERROR;

    act = (*pl)->sig;

    do
    {
        if(cmp(act->dato, datoOrigen) == 0)
        encontrado = 1;
        else
        act = act->sig;

    } while(act != (*pl)->sig && !encontrado);

    if(!encontrado)
        return ERROR;

    for(i = 0; i < pasos; i++)
    {
        if(direccion == 1)
            act = act->sig;
        else
            act = act->ant;
    }

    memcpy(datoDestino, act->dato, MINI(tam, act->tamDato));

    return TODO_OK;
}

int vaciarListaDoble(tListaDobleC* pld)
{
    int cant=0;
    tNodoLista* act = *pld;

    if(act)
    {
        while(act->ant)
            act=act->ant;
        while(act)
        {
            tNodoLista* aux = act->sig;
            free(act->dato);
            free(act);
            act=aux;
            cant++;
        }
        *pld = NULL;
    }
    return cant;
}
