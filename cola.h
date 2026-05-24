#ifndef COLA
#define COLA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERDADERO 1
#define FALSO 0

#define MIN(a,b) ((a<b) ? (a):(b))

typedef struct sNodo
{
    void* info;
    unsigned tamInfo;
    struct sNodo* sig;
}tNodo;

typedef struct
{
    tNodo* frente;
    tNodo* fondo;
}tCola;

void crearCola(tCola* cola);
void vaciarCola(tCola* cola);
int aColar(tCola* cola, const void* dato, unsigned tam);
int verPrimero(const tCola* cola, void* dato, unsigned tam);
int sacarDeCola(tCola* cola, void* dato, unsigned tam);
int colaVacia(const tCola* cola);
int colaLlena(const tCola* cola, unsigned tam);

#endif // COLA
