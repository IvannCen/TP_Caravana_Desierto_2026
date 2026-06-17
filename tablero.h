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
    int posicion;
    char componente;
    int cantBandidos;
    int hayJugador;
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
    int estadoPartida;
    tBandido* vecBandidos;
    int cantBandidosActivos;
    int posAnteriorJugador;
} tJuego;

typedef struct
{
    char movimientoDe;
    int idBandido;
    int direccion;
    int pasos;
} tMovimiento;

typedef struct
{
    char direccion;
    int pasos;
} tMovHistorico;

void inicializarJuego(tJuego *juego, int cantPos, const char* vecpos);
void crearTablero(tListaDobleC *pld, int cantPos, const char* vecpos);
void crearJugador(tJugador *j, const char *nombreJ, int cantVidas);
void ubicarEntidades(tJuego* juego, tJugador* jugador, int maxBandidos);
void turno(tJugador *j, tJuego *juego);
void ponerComponentesEnTablero(tJuego *juego, tConfiguracion *config, char tipo, int cantComp, int zonaExclusion);
void ponerTodosLosComponentes(tJuego *juego, tConfiguracion *config);
int pedirDireccion();
int tirarDado();
void moverJugadorConRebote(tJugador *j, int pasos, int direccion, tJuego *juego);
void moverJugador(tJugador *j, tJuego *juego, int pasos, int direccion);
void mostrarCasillero(const void *a);
void registrarMovimiento(tJugador *j, tMovimiento *mov);
void encolarMovimientosBandidos(tCola* cola, tJuego* juego, tJugador* jugador);
void moverBandidoSinRebote(tBandido* b, int pasos, int direccion, tJuego* juego);
void ponerEnColarMovimientoJugador(tCola *cola, int direccion, int pasos);
void procesarCola(tCola *cola, tJugador *j, tJuego *juego);
void aplicarEfectos(tJugador *j, tJuego *juego);
int cmpCasillero(const void *a, const void *b);
void mostrarMovimientoHistorial(const void *a);


#endif // TABLERO_H_INCLUDED
