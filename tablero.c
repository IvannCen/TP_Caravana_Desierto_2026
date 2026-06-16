#include "tablero.h"
#include "TDA_ListaDobleCircular.h"
#include "TDA_ListaSimple.h"
#include "TDA_ColaDinamica.h"


void inicializarJuego(tJuego *juego, int cantPos, const char* vecpos)   ///MODIFICADA
{
    crearListaDoble(&juego->tablero);
    crearCola(&juego->colaMovimientos);
    crearTablero(&juego->tablero, cantPos, vecpos);

    juego->posInicio = 1;
    juego->posSalida = cantPos;
    juego->estadoPartida = 0;
}

void crearTablero(tListaDobleC *pld, int cantPos, const char* vecpos)   ///MODIFICADA
{
    tCasillero casillero;
    int i;

    for(i = 0; i < cantPos; i++)
    {
        casillero.posicion = i + 1;
        casillero.componente = *(vecpos + i);
        casillero.cantBandidos = 0;
        casillero.hayJugador = 0;

        if(i == 0)
            casillero.componente = 'I';
        else if(i == cantPos - 1)
            casillero.componente = 'S';
        else
            casillero.componente = *(vecpos + i);

        ponerAlFinalEnListaCircular(pld, &casillero, sizeof(tCasillero));
    }
}

void crearJugador(tJugador *j, const char *nombreJ, int cantVidas)
{
    strcpy(j->nombre, nombreJ);
    j->cantVidas = cantVidas;
    j->pierdeTurno = 0;
    j->protegido = 0;
    j->puntos = 0;
    crearLista(&j->historialMovimientos);
}

void ubicarEntidades(tJuego* juego, tJugador* jugador, int maxBandidos) ///MODIFICADA
{
    int i;
    tCasillero casillero;
    tBandido *b;

    juego->vecBandidos = (tBandido*)malloc(maxBandidos * sizeof(tBandido));
    if(!juego->vecBandidos)
        return;

    juego->cantBandidosActivos = 0;

    for(i = 1; i <= juego->posSalida; i++)
    {
        casillero.posicion = i;

        if(buscarEnListaCircular(&juego->tablero, &casillero, sizeof(tCasillero), cmpCasillero))
        {
            if(casillero.componente == 'I')
            {
                jugador->posActual = casillero.posicion;
                casillero.hayJugador = 1;
                juego->posAnteriorJugador = casillero.posicion;

                actualizarEnListaCircular(&juego->tablero, &casillero, sizeof(tCasillero), cmpCasillero);
            }
            else if(casillero.componente == 'B')
            {
                b = &juego->vecBandidos[juego->cantBandidosActivos];
                b->id = juego->cantBandidosActivos + 1;
                b->vivo = 1;

                b->posActual = casillero.posicion;
                casillero.cantBandidos = 1;
                juego->cantBandidosActivos++;

                actualizarEnListaCircular(&juego->tablero, &casillero, sizeof(tCasillero), cmpCasillero);
            }
        }
    }
}

int pedirDireccion()
{
    int dir;

    do
    {
        printf("\n=== Indique la dirección del movimiento ===\n");
        printf("1. Adelante.\n");
        printf("2. Atrás.\n");
        printf("Opcion: ");
        scanf("%d", &dir);
        //para limpiar el buffer porque me toma el enter como una nueva opcion
        while(getchar() != '\n');
        system("cls");
        if (dir != 1 && dir != 2)
        {
            printf("\nOpción invalida. Por favor, ingrese 1 o 2.\n");
        }
        else
        {
            printf("\n");
        }

    }
    while(dir != 1 && dir != 2);

    return dir;
}

int tirarDado()
{
    return (rand() % 6) + 1;
}

void moverJugadorConRebote(tJugador *j, int pasos, int direccion, tJuego *juego)    ///MODIFICADA
{
    int pasosHastaLimite, sobrantes, direccionRebote;
    tCasillero actual, destino, limite;
    actual.posicion = j->posActual;

    buscarEnListaCircular(&juego->tablero, &actual, sizeof(tCasillero), cmpCasillero);
    actual.hayJugador = 0;
    actualizarEnListaCircular(&juego->tablero, &actual, sizeof(tCasillero), cmpCasillero);

    if(direccion == 1)
        pasosHastaLimite = juego->posSalida - actual.posicion;
    else
        pasosHastaLimite = actual.posicion - 1;

    if (pasos <= pasosHastaLimite)
    {
        obtenerElementoDesplazado(&juego->tablero, &actual, pasos, direccion, &destino, sizeof(tCasillero), cmpCasillero);
    }
    else
    {
        sobrantes = pasos - pasosHastaLimite;

        if(direccion == 1)
            direccionRebote = 2;
        else
            direccionRebote = 1;

        // Tramo 1: Le pedimos al TDA viajar exactamente hasta el extremo ('S' o 'I')
        obtenerElementoDesplazado(&juego->tablero, &actual, pasosHastaLimite, direccion, &limite, sizeof(tCasillero), cmpCasillero);

        // Tramo 2: Desde ese extremo, le pedimos al TDA rebotar los pasos sobrantes en direcci�n contraria
        obtenerElementoDesplazado(&juego->tablero, &limite, sobrantes, direccionRebote, &destino, sizeof(tCasillero), cmpCasillero);
    }

    destino.hayJugador = 1;
    actualizarEnListaCircular(&juego->tablero, &destino, sizeof(tCasillero), cmpCasillero);

    j->posActual = destino.posicion;
}

void ponerEnColarMovimientoJugador(tCola *cola, int direccion, int pasos)
{
    tMovimiento mov;
    mov.movimientoDe = 'J';
    mov.idBandido = -1;
    mov.direccion = direccion;
    mov.pasos = pasos;
    ponerEnCola(cola, &mov, sizeof(tMovimiento));
}

void procesarCola(tCola *cola, tJugador *j, tJuego *juego)  ///MODIFICADA
{
    tMovimiento mov;
    tMovHistorico movH;
    tBandido* b;

    while (!colaVacia(cola))
    {
        sacarDeCola(cola, &mov, sizeof(tMovimiento));

        if (mov.movimientoDe == 'J')
        {
            moverJugadorConRebote(j, mov.pasos, mov.direccion, juego);
            aplicarEfectos(j, juego);

            movH.direccion = mov.direccion;
            movH.pasos = mov.pasos;
            ponerEnListaAlFinal(&j->historialMovimientos, &movH, sizeof(tMovHistorico));
        }
        else if(mov.movimientoDe == 'B')
        {
            b = &juego->vecBandidos[mov.idBandido - 1]; // idBandido arranca en 1, el índice en 0

            if(b->vivo)
            {
                moverBandidoSinRebote(b, mov.pasos, mov.direccion, juego);

                if(b->posActual == j->posActual)
                {
                    if(j->protegido > 0)
                        printf("\n¡Un bandido cayo en tu casillero, pero el Oasis te salvo. Bien ahi che!\n");
                    else
                    {
                        printf("\n¡Un bandido te atrapo! Perdes una vida y volves al inicio.\n\n");

                        j->cantVidas--;
                        b->vivo = 0;

                        j->pierdeTurno = 0;
                        j->protegido = 0;

                        tCasillero casDestino;
                        casDestino.posicion = j->posActual;
                        buscarEnListaCircular(&juego->tablero, &casDestino, sizeof(tCasillero), cmpCasillero);

                        casDestino.cantBandidos--;
                        if (casDestino.componente == 'B' && casDestino.cantBandidos == 0)
                            casDestino.componente = '.';

                        casDestino.hayJugador = 0;
                        actualizarEnListaCircular(&juego->tablero, &casDestino, sizeof(tCasillero), cmpCasillero);

                        j->posActual = juego->posInicio;

                        tCasillero casInicio;
                        casInicio.posicion = juego->posInicio;
                        buscarEnListaCircular(&juego->tablero, &casInicio, sizeof(tCasillero), cmpCasillero);

                        casInicio.hayJugador = 1;
                        actualizarEnListaCircular(&juego->tablero, &casInicio, sizeof(tCasillero), cmpCasillero);

                        if(j->cantVidas == 0)
                        {
                            printf("Te quedaste sin vidas... mas suerte la proxima.\n");
                            juego->estadoPartida = -1;
                        }
                    }
                }
            }
        }
    }
}

void registrarMovimiento(tJugador *j, tMovimiento *mov)
{
    tMovHistorico movH;

    if(mov->direccion == 1)
        movH.direccion = 'F';
    else
        movH.direccion = 'B';

    movH.pasos = mov->pasos;

    ponerEnListaAlFinal(&j->historialMovimientos, &movH, sizeof(tMovHistorico));
}


void aplicarEfectos(tJugador *j, tJuego *juego)
{
    tCasillero c;
    c.posicion = j->posActual;
    buscarEnListaCircular(&juego->tablero, &c, sizeof(tCasillero), cmpCasillero);

    if (c.componente == 'O')
    {
        printf("\n¡Llegaste a un Oasis seguro! Consigues protección por este turno\n");
        j->protegido = 2;
    }

    if (c.cantBandidos > 0)
    {
        if (j->protegido > 0)
            printf("\n¡Hay un bandido en este casillero, pero la protección del Oasis te hace inmune!\n");
        else
        {
            printf("\n¡Te atrapo un bandido! Perdes una vida y volves al inicio.\n");
            j->cantVidas--;

            int k = 0;
            int bandidoEncontrado = 0;
            while (k < juego->cantBandidosActivos && !bandidoEncontrado)
            {
                if (juego->vecBandidos[k].vivo == 1 && juego->vecBandidos[k].posActual == j->posActual)
                {
                    juego->vecBandidos[k].vivo = 0;
                    c.cantBandidos--;

                    if (c.componente == 'B' && c.cantBandidos == 0)
                        c.componente = '.';

                    bandidoEncontrado = 1;
                }
                k++;
            }

            c.hayJugador = 0;
            actualizarEnListaCircular(&juego->tablero, &c, sizeof(tCasillero), cmpCasillero);

            j->posActual = juego->posInicio;

            tCasillero casInicio;
            casInicio.posicion = juego->posInicio;
            buscarEnListaCircular(&juego->tablero, &casInicio, sizeof(tCasillero), cmpCasillero);
            casInicio.hayJugador = 1;
            actualizarEnListaCircular(&juego->tablero, &casInicio, sizeof(tCasillero), cmpCasillero);

            if (j->cantVidas == 0)
            {
                printf("Perdiste, te quedaste sin vidas... mas suerte la proxima.\n");
                juego->estadoPartida = -1;
            }
            return;
        }
    }

    switch (c.componente)
    {
        case 'S':
            printf("\n[VICTORIA] ¡Lograste llegar a la ciudad refugio de forma segura!\n");
            juego->estadoPartida = 1;
            break;

        case 'P':
            printf("\n[PREMIO] ¡Obtuviste un punto extra!\n");
            j->puntos++;
            c.componente = '.';
            break;

        case 'V':
            printf("\n[VIDA] ¡Encontraste suministros médicos! Obtuviste una vida extra.\n");
            j->cantVidas++;
            c.componente = '.';
            break;

        case 'T':
            if (j->protegido > 0)
            {
                printf("\n[!] Una tormenta de arena te rodea, ¡pero el escudo del Oasis te mantiene firme!\n");
            }
            else
            {
                printf("\n[TORMENTA] ¡Estas atrapado en una tormenta! Perderas tu proximo turno.\n");
                j->pierdeTurno = 1;
            }
            break;

        case '.':
        case 'I':
        case 'O':
            break;
    }

    actualizarEnListaCircular(&juego->tablero, &c, sizeof(tCasillero), cmpCasillero);
}

void turno(tJugador *j, tJuego *juego)  ///MODIFICADA
{
    int pasos, direccion;

    if (j->pierdeTurno)
    {
        printf("\n\n¡Los bandidos se mueven mientras estas aturdido!\n");
//        for(int i=0; i<3; i++)
//        {
//            printf(".\n");
//        }
//        printf("\n");
        j->pierdeTurno = 0;
        encolarMovimientosBandidos(&juego->colaMovimientos, juego, j);
        procesarCola(&juego->colaMovimientos, j, juego);
        mostrarListaDeIzqADer(&juego->tablero, mostrarCasillero);
        return;
    }

    if (j->protegido>0)
        j->protegido--;

    printf("\nVidas actuales: ");
    for(int k = 0; k < j->cantVidas; k++)
    {
        printf("\xE2\x99\xA5 ");
    }
    printf("(x%d)", j->cantVidas);

    printf("\nPuntos actuales: ");
    for(int k = 0; k < j->puntos; k++)
    {
        printf("\xE2\x98\x85 ");
    }
    printf("(x%d)\n", j->puntos);
    printf("\nPresione ENTER para arrojar el dado virtual...");
    getchar();

    pasos = tirarDado();
    printf("\nEl dado cayó en: %d\n", pasos);

    direccion = pedirDireccion();

    // encolar movimientos
    ponerEnColarMovimientoJugador(&juego->colaMovimientos, direccion, pasos);
    encolarMovimientosBandidos(&juego->colaMovimientos, juego, j);

    // procesar cola
    procesarCola(&juego->colaMovimientos, j, juego);

    juego->posAnteriorJugador = j->posActual; // guardo la posicion donde qued� el jugador al terminar el turno, asi los bandidos la usan para moverse

    mostrarListaDeIzqADer(&juego->tablero, mostrarCasillero);
}

void mostrarCasillero(const void *a)
{
    tCasillero casillero = *(const tCasillero *)a;

    // 1. Agregamos la impresi�n de la posici�n al inicio
    // El "%02d" asegura que los n�meros del 1 al 9 tengan un cero adelante (01, 02...)
    printf("%02d: ", casillero.posicion);

    // 2. El resto de tu l�gica queda intacta
    if(casillero.hayJugador)
    {
        if(casillero.componente == '.')
            printf("[J] ");
        else
            printf("[%c J] ", casillero.componente);
    }
    else if(casillero.cantBandidos > 0)
    {
        if(casillero.componente == '.' || casillero.componente == 'B')
            printf("[B] ");
        else
            printf("[%c B] ", casillero.componente);
    }
    else
    {
        printf("[%c] ", casillero.componente);
    }

    printf("\n");
}

void encolarMovimientosBandidos(tCola* cola, tJuego* juego, tJugador* jugador)  ///MODIFICADA
{
    int i, pasosB, posBandido, posJugadorAnterior;
    tMovimiento mov;

    for(i=0; i<juego->cantBandidosActivos; i++)
    {
        if(juego->vecBandidos[i].vivo)
        {
            pasosB = tirarDado();

            posBandido = juego->vecBandidos[i].posActual;
            posJugadorAnterior = juego->posAnteriorJugador;

            mov.movimientoDe = 'B';
            mov.idBandido = juego->vecBandidos[i].id;
            mov.direccion = (posJugadorAnterior > posBandido) ? 1 : (posJugadorAnterior < posBandido) ? 2 : (rand() %2 + 1);
            mov.pasos = pasosB;
            ponerEnCola(cola, &mov, sizeof(tMovimiento));

        }
    }
}

void moverBandidoSinRebote(tBandido* b, int pasos, int direccion, tJuego* juego)    ///MODIFICADA
{
    tCasillero actual, destino;

    actual.posicion = b->posActual;
    buscarEnListaCircular(&juego->tablero, &actual, sizeof(tCasillero), cmpCasillero);
    actual.cantBandidos--;

    if(actual.componente == 'B' && actual.cantBandidos == 0)
        actual.componente = '.';

    actualizarEnListaCircular(&juego->tablero, &actual, sizeof(tCasillero), cmpCasillero);

    obtenerElementoDesplazado(&juego->tablero, &actual, pasos, direccion, &destino, sizeof(tCasillero), cmpCasillero);

    destino.cantBandidos++;

    actualizarEnListaCircular(&juego->tablero, &destino, sizeof(tCasillero), cmpCasillero);

    b->posActual = destino.posicion;
}

int cmpCasillero(const void *a, const void *b)  ///NUEVA
{
    tCasillero *casA = (tCasillero *)a;
    tCasillero *casB = (tCasillero *)b;
    return casA->posicion - casB->posicion;
}

void mostrarMovimientoHistorial(const void *a)
{
    const tMovHistorico *movH = (const tMovHistorico *)a;

    if (movH->direccion == 1)
        printf("F%d ", movH->pasos);
    else
        printf("B%d ", movH->pasos);
    printf("\n");
}
