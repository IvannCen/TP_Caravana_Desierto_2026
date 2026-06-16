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
    int posActual;
    tListaSimple historialMovimientos;
} tJugador;

typedef struct
{
    int id;
    int posActual;
    int vivo;
} tBandido;

typedef struct
{
    tListaDobleC tablero;
    int posInicio;
    int posSalida;
    tCola colaMovimientos;
    tListaSimple bandidos;
    int IdBandido;
    int estadoPartida; /// 1 victoria, 0 sigue jugando, -1 perdio

    //agrego  nuevas variables para almacenar los bandidos y para saber cuantos hay activos
    tBandido* vecBandidos;
    int cantBandidosActivos;

    int posAnteriorJugador; //para guardar donde estuvo el jugador y con eso que los bandidos se muevan
} tJuego;

typedef struct
{
    char movimientoDe; /// J o B
    int idBandido;
    int direccion;
    int pasos;
} tMovimiento;

typedef struct
{
    char direccion;
    int pasos;
} tMovHistorico;

/// TABLERO Y CREAR JUGADOR
void inicializarJuego(tJuego *juego, int cantPos, const char* vecpos);
void crearTablero(tListaDobleC *pld, int cantPos, const char* vecpos);
void crearJugador(tJugador *j, const char *nombreJ, int cantVidas);

/// agrego una funcion para la ubicacion de las entidades en el tablero
void ubicarEntidades(tJuego* juego, tJugador* jugador, int maxBandidos);
void turno(tJugador *j, tJuego *juego);

void ponerComponentesEnTablero(tJuego *juego, tConfiguracion *config, char tipo, int cantComp, int zonaExclusion);
void ponerTodosLosComponentes(tJuego *juego, tConfiguracion *config);

int pedirDireccion();
int tirarDado();

/// MOVIMIENTO DE JUGADOR
void moverJugadorConRebote(tJugador *j, int pasos, int direccion, tJuego *juego);
void moverJugador(tJugador *j, tJuego *juego, int pasos, int direccion);
void mostrarCasillero(const void *a);
void registrarMovimiento(tJugador *j, tMovimiento *mov);

/// MOVIMIENTO DE BANDIDO
void encolarMovimientosBandidos(tCola* cola, tJuego* juego, tJugador* jugador);
void moverBandidoSinRebote(tBandido* b, int pasos, int direccion, tJuego* juego);

/// COLA DE MOVIMIENTO
void ponerEnColarMovimientoJugador(tCola *cola, int direccion, int pasos);
void procesarCola(tCola *cola, tJugador *j, tJuego *juego);

void aplicarEfectos(tJugador *j, tJuego *juego);

int cmpCasillero(const void *a, const void *b);
void mostrarMovimientoHistorial(const void *a);


#endif // TABLERO_H_INCLUDED
