#include <windows.h> //PARA LOS ACENTOS
#include "configuracion.h"
#include "tablero.h"
#define nomArch "config.txt"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));

    tJugador jugador;
    tJuego juego;
    int opcionElegida;
    char nombre[50];

    tArbolBinBusq indiceJugadores;
    crearArbolBinBusq(&indiceJugadores);
    cargarIndiceBinario(&indiceJugadores, "indice.dat");

    // ACA SE CARGA LA CONFIG DEL MAPA
    tConfiguracion config;
    if(!cargarConfiguracion(&config, nomArch))
    {
        printf("Error en el archivo de configuracion...");
        return 1;
    }

    // MENU
    do
    {
        opcionElegida = mostrarMenuPrincipal();

        switch(opcionElegida)
        {
            case 1:
                system("cls");
                printf("\n=== NUEVA PARTIDA ===\n\n");
                printf("Ingrese su nombre: ");
                fflush(stdin);
                scanf("%49s", nombre);
                while(getchar() != '\n');

                int idJugadorActual;
                long posArchivoActual;
                buscarODarDeAltaJugador(&indiceJugadores, nombre, "jugadores.dat", &idJugadorActual, &posArchivoActual);

                system("cls");

                // Creamos un escenario nuevo para ESTA partida en particular
                char* posiciones = crearVecPos(config.cantidad_posiciones);

                ubicacionAleatoria(posiciones, config.cantidad_posiciones, BANDIDO, config.maximo_bandidos);
                ubicacionAleatoria(posiciones, config.cantidad_posiciones, PREMIO, config.maximo_premios);
                ubicacionAleatoria(posiciones, config.cantidad_posiciones, VIDA_EXTRA, config.maximo_vidas_extra);
                ubicacionAleatoria(posiciones, config.cantidad_posiciones, OASIS, config.maximo_oasis);
                ubicacionAleatoria(posiciones, config.cantidad_posiciones, TORMENTA, config.maximo_tormentas);

                inicializarJuego(&juego, config.cantidad_posiciones, posiciones);

                crearJugador(&jugador, nombre, config.vidas_inicio);

                ubicarEntidades(&juego, &jugador, config.maximo_bandidos);

                guardarEscenario(posiciones, config.cantidad_posiciones, "caravana.txt");

                free(posiciones);

                int turnosJugador = 0;

                mostrarListaDeIzqADer(&juego.tablero, mostrarCasillero);

                // Bucle de LA PARTIDA
                while(juego.estadoPartida == 0)
                {
                    turno(&jugador, &juego);
                    turnosJugador++;
                }

                guardarPuntaje(jugador.nombre, jugador.puntos);
                registrarNuevaPartida("partidas.dat", idJugadorActual, jugador.puntos, turnosJugador);

                if(juego.vecBandidos)
                {
                    free(juego.vecBandidos);
                    juego.vecBandidos = NULL;
                }

                printf("\nPartida finalizada.\n");
                printf("\nHistorial de movimientos:\n");
                mostrarLista(&jugador.historialMovimientos, mostrarMovimientoHistorial);

                vaciarLista(&jugador.historialMovimientos);
                vaciarCola(&juego.colaMovimientos);
                vaciarListaDoble(&juego.tablero);

                system("pause");
                break;

            case 2:
                system("cls");
                printf("\n=== RANKING DE JUGADORES ===\n\n");

                mostrarRanking();

                printf("\n");
                system("pause");
                break;

            case 3:
                system("cls");
                printf("==============================================\n");
                printf("Saliendo del juego... ¡Hasta la próxima!\n");
                printf("==============================================\n");

                guardarIndiceBinario(&indiceJugadores, "indice.dat");
                vaciarArbolBinBusq(&indiceJugadores);

                break;

            default:
                printf("\nOpción invalida, por favor ingresar un número entre 1 y 3.\n\n");
                system("pause");
        }

    } while (opcionElegida != 3);

    return 0;
}
