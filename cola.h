#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))
#define COLA_VACIA 3
#define ERROR_SIN_MEM 2
#define TODO_OK 1

typedef struct sNodo
{
    void *dato;
    unsigned tamDato;
    struct sNodo *sig;
}tNodo;

typedef struct
{
    tNodo *pri;
    tNodo *ult;
}tCola;

void crearCola(tCola *c);
int colaVacia(const tCola *c);
int colaLlena(const tCola *c, unsigned tam);
int ponerEnCola(tCola *c, const void *dato, unsigned tam);
int sacarDeCola(tCola *c, void *dato, unsigned tam);
int verPrimero(const tCola *c, void *dato, unsigned tam);
void vaciarCola(tCola *c);
#endif // COLA_H_INCLUDED
