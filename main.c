#include <windows.h>
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
    tConfiguracion config;
    if(!cargarConfiguracion(&config, nomArch))
    {
        printf("Error en el archivo de configuracion...");
        return 1;
    }
    if(!validarConfiguracion(&config))
    {
        printf("El mapa que se intentará crear no tiene solución.\nPor favor, modifique 'config.txt' con valores logicos antes de jugar.\n");
        system("pause");
        return 1;
    }
    do
    {
        opcionElegida = mostrarMenuPrincipal();

        switch(opcionElegida)
        {
        case 1:
            system("cls");
            printf("\n=== NUEVA PARTIDA ===\n\n");
            printf("[!] Recuerde que el registro distingue entre mayusculas y minusculas.\n");
            printf("Ingrese su nombre: ");
            fflush(stdin);
            scanf("%49s", nombre);
            while(getchar() != '\n');

            int idJugadorActual;
            long posArchivoActual;
            buscarODarDeAltaJugador(&indiceJugadores, nombre, "jugadores.dat", &idJugadorActual, &posArchivoActual);

            system("cls");

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

            while(juego.estadoPartida == 0)
            {
                turno(&jugador, &juego);
                turnosJugador++;
            }

            if (juego.estadoPartida == -1)
            {
                printf("\nComo no lograste llegar a la ciudad refugio, perdiste los puntos acumulados en este intento.\n");
                jugador.puntos = 0;
            }
            else if (juego.estadoPartida == 1)
                printf("\n¡Sobreviviste! Tus %d puntos serán sumados a tu perfil.\n", jugador.puntos);

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
            printf("==============================================\n\t\tREGLAS DEL JUEGO\n==============================================\n\n");
            printf("OBJETIVO PRINCIPAL:\nTu misión es atravesar el mapa de forma segura y llegar a la Ciudad Refugio (S) desde el Campamento Inicial (I). \nLa partida finaliza si lográs llegar a la meta o si perdés todas tus vidas en el intento.\n\n");
            printf("MOVIMIENTO DE LA CARAVANA:\nEn cada turno arrojarás un dado virtual (del 1 al 6) y elegirás la dirección de tu movimiento: adelante o atrás. Debés llegar a la salida con la cantidad exacta de pasos; si tu tirada supera la posición de la Ciudad Refugio, rebotarás y retrocederás los pasos sobrantes.\n\n");
            printf("LOS BANDIDOS:\nLos bandidos (B) patrullan la ruta y se mueven de forma autónoma después de tu turno, intentando darte caza. Si caés en su misma casilla te atraparán y si un bandido te alcanza moviendose hacia vos, sufrirás una emboscada. Como consecuencia perderás una vida, volverás al Campamento Inicial y el bandido morirá.\n\n");
            printf("TERRENO Y PRIORIDAD DE EVENTOS:\nEl desierto está lleno de peligros y tesoros. Al finalizar tu movimiento en un casillero, los efectos del entorno se aplican con un orden de prioridad estricto:\n\n");
            printf("1-Prioridad Máxima - OASIS (O): Es el lugar mas seguro. Te otorga un escudo de inmunidad inmediata para el turno actual y el siguiente. Si un bandido te estaba esperando en el oasis, o si intenta atraparte en tu proximo turno, el escudo te salvará y no sufrirás ningun daño.\n\n");
            printf("2-Prioridad Alta - ATRAPADA/EMBOSCADA (B): Si no tenés la inmunidad del oasis y hay un bandido esperandote en tu casillero o te cae encima al perseguirte, sufrís el ataque. La emboscada es tan rapida que anula cualquier otro efecto del terreno (no recogés premios ni sufrís el clima, simplemente volvés al inicio).\n\n");
            printf("3-Prioridad Media - TORMENTA (T): Si caes acá sin la protección del oasis, la arena te atrapa y perdés tu proximo turno. ¡Tené cuidado! Durante tu aturdimiento, los bandidos seguiran tirando sus dados y moviendose libremente hacia tu posición.\n\n");
            printf("4-Prioridad Baja - BOTIN (P o V): Si el casillero está libre de peligros, podés recoger los recursos abandonados. Podés encontrar Premios (P) que se sumarán a tu puntaje al ganar la partida, o Vidas Extra (V). Una vez recogidos, el casillero quedará vacio.\n\n");
            system("pause");
            break;
        case 4:
            system("cls");
            printf("==============================================\n");
            printf("Saliendo del juego... ¡Hasta la próxima!\n");
            printf("==============================================\n");

            guardarIndiceBinario(&indiceJugadores, "indice.dat");
            vaciarArbolBinBusq(&indiceJugadores);

            break;

        default:
            printf("\nOpción invalida, por favor ingresar un número entre 1 y 4.\n\n");
            system("pause");
        }

    }
    while (opcionElegida != 4);

    return 0;
}
