#include "cola.h"

void crearCola(tCola* cola)
{
    cola->frente=NULL;
    cola->fondo=NULL;
}

void vaciarCola(tCola* cola)
{
    tNodo* aux;
    while(cola->frente != NULL)
    {
        aux = cola->frente; //apunto al primer nodo
        cola->frente=aux->sig; //avanzo el frente al siguiente
        free(aux->info); //libero el dato
        free(aux); //libero el nodo
    }
    cola->fondo = NULL; //si no hay frente, tampoco fondo
}

int aColar(tCola* cola, const void* dato, unsigned tam)
{
    tNodo* nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(!nuevo)
        return FALSO;
    nuevo->info=malloc(tam);
    if(!nuevo->info)
    {
        free(nuevo);
        return FALSO;
    }
    memcpy(nuevo->info,dato,tam); //copio el dato al nodo
    nuevo->tamInfo=tam;
    nuevo->sig=NULL; //como entra en el fondo, no hay nadie atras
    if(cola->frente==NULL)
        cola->frente=nuevo; //si la cola esta vacia. es el primer elemento
    else
        cola->fondo->sig=nuevo; //si ya habia algo, el ultimo actual lo engancha
    cola->fondo=nuevo; //el nuevo nodo es el nuevo fondo
    return VERDADERO;
}

int verPrimero(const tCola* cola, void* dato, unsigned tam)
{
    if(cola->frente==NULL)
        return FALSO;
    memcpy(dato,cola->frente->info, MIN(tam,cola->frente->tamInfo));
    return VERDADERO;
}

int sacarDeCola(tCola* cola, void* dato, unsigned tam)
{
    if(cola->frente==NULL)
        return FALSO;
    tNodo* aux = cola->frente; //agarro al primero de la fila
    memcpy(dato,aux->info, MIN(tam,aux->tamInfo)); //copio el dato al main
    cola->frente=aux->sig; //el segundo pasa a ser el primero
    if(cola->frente==NULL)
        cola->fondo=NULL; //si saco el ultimo que queda, el fondo debe ser NULL
    free(aux->info);
    free(aux);
    return VERDADERO;
}

int colaVacia(const tCola* cola)
{
    return cola->frente == NULL;
}

int colaLlena(const tCola* cola, unsigned tam)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));
    void* info = malloc(tam);
    if(aux==NULL || info==NULL)
    {
        free(aux);
        free(info);
        return VERDADERO;
    }
    free(aux);
    free(info);
    return FALSO;
}
