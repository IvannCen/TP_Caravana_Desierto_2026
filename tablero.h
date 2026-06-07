#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuracion.h"
#include "TDA_ListaDobleCircular.h"
#include "TDA_ColaDinamica.h"
#include "TDA_ListaSimple.h"


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
    tNodoLista *posActual;
} tJugador;

typedef struct
{
    int id;
    tNodoLista *posActual;
    int vivo;
} tBandido;

typedef struct
{
    tListaDobleC tablero;
    tNodoLista *nodoInicio;
    tNodoLista *nodoSalida;
    tCola colaMovimientos;
    tListaSimple bandidos;
    int IdBandido;
    int estadoPartida; /// 1 victoria, 0 sigue jugando
} tJuego;

typedef struct
{
    char movimientoDe; /// J o B
    int idBandido;
    int direccion;
    int pasos;
} tMovimiento;

/// TABLERO Y CREAR JUGADOR
void inicializarJuego(tJuego *juego, int cantPos);
void crearTablero(tListaDobleC *pld, int cantPos);
void crearJugador(tJugador *j, const char *nombreJ, int cantVidas);
void ubicarJugador(tJuego *juego, tJugador *j);
void turno(tJugador *j, tJuego *juego);

void ponerComponentesEnTablero(tJuego *juego, tConfiguracion *config, char tipo, int cantComp, int zonaExclusion);
void ponerTodosLosComponentes(tJuego *juego, tConfiguracion *config);
void agregarBandido(tListaSimple *pl, int id, tNodoLista *posicion);

int pedirDireccion();
int tirarDado();

/// MOVIMIENTO DE JUGADOR
int pasosHastaNodo(tNodoLista *origen, tNodoLista *destino, int direccion);
void moverJugadorConRebote(tJugador *j, int pasos, int direccion, tJuego *juego);
void moverJugador(tJugador *j, int pasos, int direccion);
void mostrarCasillero(const void *a);

/// COLA DE MOVIMIENTO
void ponerEnColarMovimientoJugador(tCola *cola, int direccion, int pasos);
void procesarCola(tCola *cola, tJugador *j, tJuego *juego);

void aplicarEfectos(tJugador *j, tJuego *juego);

#endif // TABLERO_H_INCLUDED
