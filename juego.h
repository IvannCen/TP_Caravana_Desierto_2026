#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "configuracion.h"
#include "listaDoble.h"

typedef struct
{
    int numeroPosicion; // Para saber si es el casillero 1, 2, 25, etc.
    char elementoBase;  // Aca va el 'I', 'S', 'P', 'V', 'O', 'T', o '.' (lo que ya generas en vecpos)
    int hayJugador;     // 1 si el jugador esta parado aca, 0 si no
    int cantBandidos;   // Cuantos bandidos hay parados en este casillero (pueden caer varios juntos)
} tCasillero;

typedef struct
{
    int vidas;
    int puntos;
    int protegidoPorOasis; // 1 si está protegido para el prox turno, 0 si no
    int pierdeTurno;       // 1 si cayó en tormenta, 0 si no
} tJugador;

// recibe una lista vacia junto con los datos de la config y va insertando un tcasillero en cada posicion del vector con la info del mismo
void generarMapaDesdeVector(tLista* mapa, char* vecpos, int tam);
//nomas muestra el mapa
void imprimirMapaConsola(const tLista* mapa);
//funcion para mover al player
tNodo* moverJugador(tNodo* posicionActual, int dado, char direccion, FILE* archivoRegistro);
// actualiza el tcasillero donde cayó el jugador y le cambia las stats
void resolverCasillero(tNodo* posicionActual, tJugador* jugador);
#endif // JUEGO_H_INCLUDED
