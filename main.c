#include "configuracion.h"
#include "TDA_ListaDobleCircular.h"
#include "tablero.h"

#define nomArch "config.txt"

int main()
{
    srand(time(NULL));


    tJugador jugador;
    tJuego juego;
    tConfiguracion config;
    char nombre[] = "jugador1";

    cargarConfiguracion(&config, nomArch);
    inicializarJuego(&juego, config.cantidad_posiciones);
    crearJugador(&jugador, nombre, config.vidas_inicio);
    ubicarJugador(&juego, &jugador);
    ponerTodosLosComponentes(&juego, &config);

    mostrarListaDeIzqADer(&juego.tablero, mostrarCasillero);

    while(juego.estadoPartida == 0)
    {
        turno(&jugador, &juego);
    }

    return 0;
}
