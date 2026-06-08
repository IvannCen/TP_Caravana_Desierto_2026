#include "configuracion.h"
#include "TDA_ListaDobleCircular.h"
#include "tablero.h"


int main()
{
    srand(time(NULL));

    tJugador jugador;
    tJuego juego;

    tConfiguracion config;
    if(!cargarConfiguracion(&config,"config.txt"))
    {
        printf("Error en el archivo de configuracion...");
        return 1;
    }
        
    char* posiciones=crearVecPos(config.cantidad_posiciones);

    ubicacionAleatoria(posiciones,config.cantidad_posiciones,BANDIDO,config.maximo_bandidos);
    ubicacionAleatoria(posiciones,config.cantidad_posiciones,PREMIO,config.maximo_premios);
    ubicacionAleatoria(posiciones,config.cantidad_posiciones,VIDA_EXTRA,config.maximo_vidas_extra);
    ubicacionAleatoria(posiciones,config.cantidad_posiciones,OASIS,config.maximo_oasis);
    ubicacionAleatoria(posiciones,config.cantidad_posiciones,TORMENTA,config.maximo_tormentas);

    char nombre[] = "jugador1";

    inicializarJuego(&juego, config.cantidad_posiciones, posiciones);
    free(posiciones);
    crearJugador(&jugador, nombre, 3);

    ubicarEntidades(&juego, &jugador, config.maximo_bandidos);
    // ubicarJugador(&juego, &jugador);

    mostrarListaDeIzqADer(&juego.tablero, mostrarCasillero);

    while(juego.estadoPartida == 0)
    {
        turno(&jugador, &juego);
    }

    if(juego.vecBandidos)
        free(juego.vecBandidos);

    return 0;
}
