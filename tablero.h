#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TDA_ListaDobleCircular.h"

typedef struct
{
   int posicion;        /// numero de casillero en el tablero
   char componente;     /// I J . T O S, etc..
   int cantBandidos;    /// numero de bandidos en el casillero ya que puede haber varios en uno mismo
   int hayJugador;      /// para saber si el jugador se encuentra en ese casillero
} tCasillero;

typedef struct
{
    char nombre[49];
    int cantVidas;
    int puntos;
    int pierdeTurno;
    int protegido;
    tNodo *posActual;
} tJugador;

typedef struct
{
    int id;
    tNodo *posActual;
    int vivo;
} tBandido;

typedef struct
{
    tListaDobleC tablero;
    tNodo *nodoInicio;
    tNodo *nodoSalida;
} tJuego;

void inicializarJuego(tJuego *juego, int cantPos);
void crearTablero(tListaDobleC *pld, int cantPos);
void crearJugador(tJugador *j, const char *nombreJ, int cantVidas);
void ubicarJugador(tJuego *juego, tJugador *j);

int pedirDireccion();
int tirarDado();

int pasosHastaNodo(tNodo *origen, tNodo *destino, char direccion);
void moverJugadorConRebote(tJugador *j, int pasos, char direccion, tJuego *juego);
void moverJugador(tJugador *j, int pasos, char direccion);
void mostrarCasillero(const void *a);

// cosas bandidos
void ubicarBandidos(tJuego *juego, tBandido *bandidos, int cantBandidos);
void moverBandidos(tBandido *bandidos, int cantBandidos, tJugador *jugador);
#endif // TABLERO_H_INCLUDED
