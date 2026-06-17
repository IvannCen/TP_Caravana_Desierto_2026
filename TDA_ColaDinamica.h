#ifndef TDA_COLADINAMICA_H_INCLUDED
#define TDA_COLADINAMICA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))
#define COLA_VACIA 3
#define ERROR_SIN_MEM 2
#define TODO_OK 1

typedef struct sNodoCola
{
    void *dato;
    unsigned tamDato;
    struct sNodoCola *sig;
} tNodoCola;

typedef struct
{
    tNodoCola *pri;
    tNodoCola *ult;
} tCola;

void crearCola(tCola *c);
int colaVacia(const tCola *c);
int colaLlena(const tCola *c, unsigned tam);
int ponerEnCola(tCola *c, const void *dato, unsigned tam);
int sacarDeCola(tCola *c, void *dato, unsigned tam);
int verPrimero(const tCola *c, void *dato, unsigned tam);
void vaciarCola(tCola *c);

#endif // TDA_COLADINAMICA_H_INCLUDED
