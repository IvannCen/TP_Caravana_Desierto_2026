#include "configuracion.h"
#include "TDA_ListaDobleCircular.h"
#include "tablero.h"
#include <windows.h>
// Si tenés mostrarMenuPrincipal() en otro archivo, acordate de incluir su .h acá
// int mostrarMenuPrincipal();

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));

    tJugador jugador;
    tJuego juego;
    int opcionElegida;
    char nombre[50];

    // ACA SE CARGA LA CONFIG DEL MAPA
    tConfiguracion config;
    if(!cargarConfiguracion(&config, "config.txt"))
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
                printf("Ingrese su nombre:");
                fflush(stdin);
                scanf("%49s", nombre); // Leemos el nombre ingresado por el jugador
                while(getchar() != '\n');
                system("cls");
                // Creamos un escenario nuevo para ESTA partida en particular
                char* posiciones = crearVecPos(config.cantidad_posiciones);

                ubicacionAleatoria(posiciones, config.cantidad_posiciones, BANDIDO, config.maximo_bandidos);
                ubicacionAleatoria(posiciones, config.cantidad_posiciones, PREMIO, config.maximo_premios);
                ubicacionAleatoria(posiciones, config.cantidad_posiciones, VIDA_EXTRA, config.maximo_vidas_extra);
                ubicacionAleatoria(posiciones, config.cantidad_posiciones, OASIS, config.maximo_oasis);
                ubicacionAleatoria(posiciones, config.cantidad_posiciones, TORMENTA, config.maximo_tormentas);

                // Inicializamos todo
                inicializarJuego(&juego, config.cantidad_posiciones, posiciones);
                free(posiciones); // Ya está cargado en la LDE, lo liberamos

                // Usamos las vidas del config
                crearJugador(&jugador, nombre, config.vidas_inicio);

                ubicarEntidades(&juego, &jugador, config.maximo_bandidos);
                //esto imprime el mapa
                mostrarListaDeIzqADer(&juego.tablero, mostrarCasillero);

                // Bucle de LA PARTIDA
                while(juego.estadoPartida == 0)
                {
                    turno(&jugador, &juego);
                }

                // Limpiamos los bandidos al terminar la partida
                if(juego.vecBandidos)
                {
                    free(juego.vecBandidos);
                    juego.vecBandidos = NULL; // Buena práctica dejar el puntero en NULL
                }

                printf("\nPartida finalizada.\n");
                system("pause");
                break;

            case 2:
                system("cls");
                printf("\n=== RANKING DE JUGADORES ===\n");
                // FALTA IMPLEMENTAR EL ARBOL BINARIO PARA EL RANKING
                system("pause");
                break;

            case 3:
                system("cls");
                printf("==============================================\n");
                printf("\nSaliendo del juego... ¡Hasta la próxima!\n");
                printf("==============================================\n");
                break;

            default:
                printf("\nOpción invalida, por favor ingresar un número entre 1 y 3.\n\n");
                system("pause");
        }

    } while (opcionElegida != 3);

    return 0;
}
