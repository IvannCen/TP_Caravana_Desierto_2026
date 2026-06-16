#include "TDA_ArbolBinarioBusqueda.h"

void crearArbolBinBusq(tArbolBinBusq *p)
{
    *p = NULL;
}

// Inserta de forma recursiva: los menores a la izquierda, los mayores a la derecha
int insertarEnArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned tam, int (*cmp)(const void *, const void *))
{
    tNodoArbol *nue;
    int comp;

    if(*p)
    {
        comp = cmp(d, (*p)->dato);
        if(comp < 0)
            return insertarEnArbolBinBusq(&(*p)->izq, d, tam, cmp);
        else
            // si es mayor o igual lo mandamos siempre a la derecha.
            // los empates se almacenen como nodos nuevos en el arbol.
            return insertarEnArbolBinBusq(&(*p)->der, d, tam, cmp);
    }

    nue = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if(!nue)
        return ERROR;
    nue->dato = malloc(tam);
    if(!nue->dato)
    {
        free(nue);
        return ERROR;
    }

    memcpy(nue->dato, d, tam);
    nue->tamDato = tam;
    nue->izq = NULL;
    nue->der = NULL;
    *p = nue;

    return TODO_OK;
}

// Recorre recursivamente: Derecha -> Raíz -> Izquierda (Para que salga de MAYOR a MENOR)
void recorrerArbolInOrdenInverso(const tArbolBinBusq *p, void (*accion)(const void *))
{
    if(!*p)
        return;

    recorrerArbolInOrdenInverso(&(*p)->der, accion);
    accion((*p)->dato);
    recorrerArbolInOrdenInverso(&(*p)->izq, accion);
}

void vaciarArbolBinBusq(tArbolBinBusq *p)
{
    if(!*p)
        return;

    vaciarArbolBinBusq(&(*p)->izq);
    vaciarArbolBinBusq(&(*p)->der);
    free((*p)->dato);
    free(*p);
    *p = NULL;
}
