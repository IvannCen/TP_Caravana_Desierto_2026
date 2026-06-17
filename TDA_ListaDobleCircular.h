#ifndef TDA_LISTADOBLECIRCULAR_H_INCLUDED
#define TDA_LISTADOBLECIRCULAR_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINI(X,Y) ((X) > (Y) ? (X) : (Y))
#define ERROR 0
#define TODO_OK 1

typedef struct sNodoLista
{
    void *dato;
    unsigned tamDato;
    struct sNodoLista *ant;
    struct sNodoLista *sig;
} tNodoLista;

typedef tNodoLista *tListaDobleC;

void crearListaDoble(tListaDobleC *pld);
int vaciarListaDoble(tListaDobleC* pld);
int listaDobleVacia(const tListaDobleC *pld);
int ponerAlFinalEnListaCircular(tListaDobleC *pld, const void *dato, unsigned tam);

int buscarEnListaCircular(tListaDobleC *pl, void *dato, unsigned tam, int (*cmp)(const void*, const void*));
int actualizarEnListaCircular(tListaDobleC *pl, const void *dato, unsigned tam, int (*cmp)(const void*, const void*));
int obtenerElementoDesplazado(tListaDobleC *pl, const void *datoOrigen, int pasos, int direccion, void *datoDestino, unsigned tam, int (*cmp)(const void*, const void*));

void mostrarListaDeIzqADer(tListaDobleC *pld, void (* mostrar)(const void *a));
void mostrarCasillero(const void *a);

#endif // TDA_LISTADOBLECIRCULAR_H_INCLUDED
