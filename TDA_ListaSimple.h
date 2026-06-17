#ifndef TDA_LISTASIMPLE_H_INCLUDED
#define TDA_LISTASIMPLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR 0
#define TODO_OK 1
#define LISTA_VACIA 2
#define MINIMO(X,Y) ((X) <= (Y) ? (X) : (Y))

typedef struct sNodo
{
    void *dato;
    unsigned tamDato;
    struct sNodo *sig;
} tNodoListaS;

typedef tNodoListaS *tListaSimple;

void crearLista(tListaSimple *l);
int listaVacia(const tListaSimple *l);
int listaLlena(const tListaSimple *l,const void *dato, unsigned tam);
void vaciarLista(tListaSimple *l);
int ponerEnListaAlFinal(tListaSimple *l, const void *dato, unsigned tam);
int sacarDeListaAlPrincipio(tListaSimple *l, void *dato, unsigned tam);
int verPrimeroEnLista(const tListaSimple *l, void *dato, unsigned tam);
void mostrarLista(const tListaSimple *pl, void (*mostrar)(const void *a));

#endif // TDA_LISTASIMPLE_H_INCLUDED
