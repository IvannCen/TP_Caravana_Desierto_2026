#include "tablero.h"
#include "TDA_ListaDobleCircular.h"


void inicializarJuego(tJuego *juego, int cantPos, const char* vecpos)
{
    crearListaDoble(&juego->tablero);
    crearCola(&juego->colaMovimientos);
    crearTablero(&juego->tablero, cantPos, vecpos);

    juego->nodoInicio = juego->tablero->sig;
    juego->nodoSalida = juego->tablero;
    juego->estadoPartida = 0;
}

void crearTablero(tListaDobleC *pld, int cantPos, const char* vecpos)
{
    tCasillero casillero;
    int i;

    for(i = 0; i < cantPos; i++)
    {
        casillero.posicion = i+1;
        casillero.componente = *(vecpos + i);
        casillero.cantBandidos = 0;
        casillero.hayJugador = 0;

        ponerAlFinalEnListaCircular(pld, &casillero, sizeof(tCasillero));
    }

    ((tCasillero*)(*pld)->sig->dato)->componente = 'I';

    ((tCasillero*)(*pld)->dato)->componente = 'S';
}

void crearJugador(tJugador *j, const char *nombreJ, int cantVidas)
{
    strcpy(j->nombre, nombreJ);
    j->cantVidas = cantVidas;
    j->pierdeTurno = 0;
    j->protegido = 0;
    j->puntos = 0;
}

// void ubicarJugador(tJuego *juego, tJugador *j)
// {
//     tNodoLista *inicio = juego->tablero->sig;
//     tCasillero *casillero = (tCasillero *)inicio->dato;
//     casillero->hayJugador = 1;
//     j->posActual = inicio;
// }

void ubicarEntidades(tJuego* juego, tJugador* jugador, int maxBandidos)
{
    juego->vecBandidos = (tBandido*)malloc(maxBandidos * sizeof(tBandido));
    if(!juego->vecBandidos)
        return;
    juego->cantBandidosActivos = 0;
    tNodoLista* aux = juego->tablero;
    do
    {
        tCasillero* cas = (tCasillero*)aux->dato;
        if(cas->componente == 'I')
        {
            jugador->posActual = aux;
            cas->hayJugador = 1;
        }
        else if(cas->componente == 'B')
        {
            tBandido* b = &juego->vecBandidos[juego->cantBandidosActivos];
            b->id = juego->cantBandidosActivos + 1;
            b->vivo = 1;
            b->posActual = aux;
            cas->cantBandidos = 1;
            juego->cantBandidosActivos++;
        }
        aux = aux->sig;
    }while(aux != juego->tablero);
}

int pedirDireccion()
{
    int dir;

    do
    {
        printf("\n===Direccion del movimiento===\n");
        printf("1. Adelante\n");
        printf("2. Atras\n");
        printf("Opcion: ");
        scanf("%d", &dir);
        //para limpiar el buffer porque me toma el enter como una nueva opcion
        while(getchar() != '\n');

        printf("\n");

    }
    while(dir != 1 && dir != 2);

    return dir;
}

int tirarDado()
{
    return (rand() % 6) + 1;
}


// int pasosHastaNodo(tNodoLista *origen, tNodoLista *destino, int direccion)
// {
//     int pasos = 0;
//     tNodoLista *actual = origen;
//     do
//     {
//         if (direccion == 1)
//             actual = actual->sig;
//         else
//             actual = actual->ant;
//         pasos++;
//     }
//     while (actual != destino && actual != origen);
//     return pasos;
// }

void moverJugadorConRebote(tJugador *j, int pasos, int direccion, tJuego *juego)
{
    tCasillero *casActual = (tCasillero *)j->posActual->dato;
    int posActual = casActual->posicion;

    tCasillero* casSalida = (tCasillero*)juego->nodoSalida->dato;
    int cantPosiciones = casSalida->posicion;


    int pasosHastaLimite;
    int sobrantes;
    int direccionRebote;

    if(direccion == 1)
        pasosHastaLimite = cantPosiciones - posActual;
    else
        pasosHastaLimite = posActual - 1;

    if (pasos <= pasosHastaLimite)
        moverJugador(j, pasos, direccion);
    else
    {
        sobrantes = pasos - pasosHastaLimite;

        if(direccion == 1)
            direccionRebote = 2;
        else
            direccionRebote = 1;

        moverJugador(j, pasosHastaLimite, direccion);
        moverJugador(j, sobrantes, direccionRebote);
    }
}

void moverJugador(tJugador *j, int pasos, int direccion)
{
    int i;
    tCasillero *actual, *nuevo;

    actual = (tCasillero *)j->posActual->dato;
    actual->hayJugador = 0;

    for (i = 0; i < pasos; i++)
    {
        if (direccion == 1)
            j->posActual = j->posActual->sig;
        else
            j->posActual = j->posActual->ant;
    }

    nuevo = (tCasillero *)j->posActual->dato;
    nuevo->hayJugador = 1;
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

void procesarCola(tCola *cola, tJugador *j, tJuego *juego)
{
    tMovimiento mov;
    while (!colaVacia(cola))
    {
        sacarDeCola(cola, &mov, sizeof(tMovimiento));

        if (mov.movimientoDe == 'J')
        {
            moverJugadorConRebote(j, mov.pasos, mov.direccion, juego);
            aplicarEfectos(j, juego);
            // registrar en historial formato FX o BX
//            registrarMovimiento(j, mov.direccion, mov.pasos);
        }
        else if(mov.movimientoDe == 'B')
        {
            tBandido* b = &juego->vecBandidos[mov.idBandido - 1];
            if(b->vivo)
            {
                moverBandidoSinRebote(b, mov.pasos, mov.direccion, juego);

                tCasillero* casDestino = (tCasillero*)b->posActual->dato;
                if (casDestino->hayJugador == 1 && !j->protegido)
                {
                    printf("\n¡Un bandido (ID: %d) te ha emboscado!\n", b->id);
                    j->cantVidas--;
                    
                    b->vivo = 0; // El bandido muere tras el ataque
                    casDestino->cantBandidos--;
                    if (casDestino->componente == 'B') casDestino->componente = '.';

                    // Respawn del jugador
                    casDestino->hayJugador = 0;
                    j->posActual = juego->nodoInicio;
                    ((tCasillero *)j->posActual->dato)->hayJugador = 1;

                    if (j->cantVidas == 0)
                    {
                        printf("Perdiste, te quedaste sin vidas...\n");
                        juego->estadoPartida = -1;
                    }
                }
            }
        }
    }
}

// void aplicarEfectos(tJugador *j, tJuego *juego)
// {
//     tCasillero *c = (tCasillero *)j->posActual->dato;

//     switch (c->componente)
//     {
//     case 'S':
//         printf("Lograste llegar a la ciudad refugio !!\n");
//         juego->estadoPartida = 1;   // victoria
//         break;
//     case 'P':
//         printf("Obtuviste un punto !!\n");
//         j->puntos++;
//         c->componente = '.';   // el premio desaparece
//         break;
//     case 'V':
//         printf("Obtuviste una vida extra !!\n");
//         j->cantVidas++;
//         c->componente = '.';    // la vida tambien desaparece
//         break;
//     case 'O':
//         printf("Estas en un oasis lo que te genera proteccion !!\n");
//         j->protegido = 1;      // protecci�n para el turno siguiente
//         break;
//     case 'T':
//         printf("Estas en una tormenta lo que te hace perder el proximo turno\n");
//         if (!j->protegido)
//             j->pierdeTurno = 1;
//         break;
//     case '.':
//         break;
//     }

//     if (c->cantBandidos > 0 && !(j->protegido))
//     {
//         printf("Te atrapo un bandido\n");
//         (j->cantVidas)--;

//         // eliminar solo un bandido del casillero
// //        eliminarUnBandidoEnNodo(juego->bandidos, j->posicion);

//         c->hayJugador = 0;
//         ubicarJugador(juego, j);

//         if (j->cantVidas == 0)
//         {
//             printf("Perdiste te quedaste sin vidas, mas suerte la proxima\n");
//             juego->estadoPartida = -1;
//         }
//     }
// }

void aplicarEfectos(tJugador *j, tJuego *juego)
{
    tCasillero *c = (tCasillero *)j->posActual->dato;

    switch (c->componente)
    {
        case 'S':
            printf("Lograste llegar a la ciudad refugio !!\n\n");
            juego->estadoPartida = 1;
            break;
        case 'P':
            printf("Obtuviste un punto !!\n\n");
            j->puntos++;
            c->componente = '.';
            break;
        case 'V':
            printf("Obtuviste una vida extra !!\n\n");
            j->cantVidas++;
            c->componente = '.';
            break;
        case 'O':
            printf("Estas en un oasis lo que te genera proteccion !!\n\n");
            j->protegido = 1;
            break;
        case 'T':
            printf("Estas en una tormenta lo que te hace perder el proximo turno\n\n");
            if (!j->protegido)
                j->pierdeTurno = 1;
            break;
        case '.':
        case 'I':
            break;
    }


    if (c->cantBandidos > 0 && !(j->protegido))
    {
        printf("¡Te atrapo un bandido!\n\n");
        (j->cantVidas)--;

        int k=0;
        int bandidoEncontrado = 0;

        while(k < juego->cantBandidosActivos && !bandidoEncontrado)
        {
            if(juego->vecBandidos[k].vivo && juego->vecBandidos[k].posActual == j->posActual)
            {
                juego->vecBandidos[k].vivo = 0; // El bandido muere tras el ataque
                c->cantBandidos--;
                if (c->componente == 'B') 
                    c->componente = '.';
                
                bandidoEncontrado = 1;
            }
            k++;
        }

        c->hayJugador = 0; // desaparece de la casilla donde fue atrapado

        j->posActual = juego->nodoInicio; // el puntero al inicio

        tCasillero* casInicio = (tCasillero *)j->posActual->dato;
        casInicio->hayJugador = 1; // reaparece en el casillero 'I'

        if (j->cantVidas == 0)
        {
            printf("Perdiste te quedaste sin vidas, mas suerte la proxima\n\n");
            juego->estadoPartida = -1; // perdio
        }
    }
}

void turno(tJugador *j, tJuego *juego)
{
    int pasos, direccion;

    if (j->pierdeTurno)
    {
        printf("\nEstas atrapado en la tormenta de arena. Pierdes este turno...\n\n");

        for(int i=0; i<3; i++)
        {
            printf("\n.");
        }
        printf("\n\nLos bandidos se mueven mientras estas aturdido!\n");
        j->pierdeTurno = 0;

        encolarMovimientosBandidos(&juego->colaMovimientos, juego, j);
        procesarCola(&juego->colaMovimientos, j, juego);
        mostrarListaDeIzqADer(&juego->tablero, mostrarCasillero);

        return;
    }

    if (j->protegido)
        j->protegido = 0;

    printf("\n\nAprete ENTER para tirar el dado virtual...");
    getchar();

    pasos = tirarDado();
    printf("\nEl dado cayo en: %d\n", pasos);

    direccion = pedirDireccion();

    // encolar movimientos
    ponerEnColarMovimientoJugador(&juego->colaMovimientos, direccion, pasos);
    encolarMovimientosBandidos(&juego->colaMovimientos, juego, j);

    // procesar cola
    procesarCola(&juego->colaMovimientos, j, juego);

    // actualizar pantalla
//    mostrarTablero(juego->tablero);
    mostrarListaDeIzqADer(&juego->tablero, mostrarCasillero);
}

void mostrarCasillero(const void *a)
{
    tCasillero casillero = *(tCasillero *)a;

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
}

void encolarMovimientosBandidos(tCola* cola, tJuego* juego, tJugador* jugador)
{
    int i;
    for(i=0;i<juego->cantBandidosActivos;i++)
    {
        if(juego->vecBandidos[i].vivo)
        {
            int posB = tirarDado();

            tCasillero* casBandido = (tCasillero*)juego->vecBandidos[i].posActual->dato;
            tCasillero* casJugador = (tCasillero*)jugador->posActual->dato;

            tMovimiento mov;
            mov.movimientoDe = 'B';
            mov.idBandido = juego->vecBandidos[i].id;
            mov.direccion = (casJugador->posicion > casBandido->posicion) ? 1 : 2;
            mov.pasos = posB;
            ponerEnCola(cola, &mov, sizeof(tMovimiento));

        }
    }
}

void moverBandidoSinRebote(tBandido* b, int pasos, int direccion, tJuego* juego)
{
    tCasillero *actual, *nuevo;
    actual = (tCasillero *)b->posActual->dato;
    actual->cantBandidos--;

    if(actual->componente == 'B' && actual->cantBandidos == 0)
        actual->componente = '.';

    int i;
    for (i = 0; i < pasos; i++)
    {
        if (direccion == 1)
            b->posActual = b->posActual->sig;
        else
            b->posActual = b->posActual->ant;
    }

    nuevo = (tCasillero *)b->posActual->dato;
    nuevo->cantBandidos++;
}
