#ifndef TDA_ARBOLBINARIOBUSQUEDA
#define TDA_ARBOLBINARIOBUSQUEDA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK 1
#define ERROR 0

typedef struct sNodoArbol
{
    void* dato;
    unsigned tamDato;
    struct sNodoArbol* izq;
    struct sNodoArbol* der;
} tNodoArbol;

typedef tNodoArbol* tArbolBinBusq;

void crearArbolBinBusq(tArbolBinBusq* p);
int insertarEnArbolBinBusq(tArbolBinBusq* p, const void* d, unsigned tam, int (*cmp)(const void*, const void*));
void recorrerArbolInOrdenInverso(const tArbolBinBusq* p, void (*accion)(const void*));
int buscarEnArbolBinBusq(const tArbolBinBusq* p, void* d, unsigned tam, int (*cmp)(const void*, const void*));
void vaciarArbolBinBusq(tArbolBinBusq* p);

#endif // TDA_ARBOLBINARIOBUSQUEDA
