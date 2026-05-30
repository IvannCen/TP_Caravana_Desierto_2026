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

typedef tNodo *tListaDobleC;

void crearListaDoble(tListaDobleC *pld);
int listaVacia(const tListaDobleC *pld);
int ponerAlFinalEnListaCircular(tListaDobleC *pld, const void *dato, unsigned tam);

void mostrarListaDeIzqADer(tListaDobleC *pld, void (* mostrar)(const void *a));
void mostrarCasillero(const void *a);

#endif // TDA_LISTADOBLECIRCULAR_H_INCLUDED
