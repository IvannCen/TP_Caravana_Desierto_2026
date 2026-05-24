#ifndef TDA_LISTADOBLECIRCULAR_H_INCLUDED
#define TDA_LISTADOBLECIRCULAR_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sNodo
{
    void *dato;
    unsigned tamDato;
    struct sNodo *ant;
    struct sNodo *sig;
} tNodo;

typedef tNodo *tLista;

void crearListaDoble(tLista *pl);
int listaVacia(const tLista *pl);
int ponerAlFinalEnListaCircular(tLista *pl, const void *dato, unsigned tam);

#endif // TDA_LISTADOBLECIRCULAR_H_INCLUDED
