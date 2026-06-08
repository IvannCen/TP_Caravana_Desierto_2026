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
        casillero.posicion = i + 1;
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
            juego->posAnteriorJugador = aux; //al inicio, la anterior es el inicio
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
    }
    while(aux != juego->tablero);
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
            // registrarMovimiento(j, mov.direccion, mov.pasos);
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
                    printf("¡Un bandido te ha emboscado! Perdés una vida y volves al inicio.\n\n");
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
                        printf("Te quedaste sin vidas.\n\n");
                        printf("GAME OVER.\n\n");
                        juego->estadoPartida = -1;
                    }
                }
            }
        }
    }
}

void aplicarEfectos(tJugador *j, tJuego *juego)
{
    tCasillero *c = (tCasillero *)j->posActual->dato;

    switch (c->componente)
    {
    case 'S':
        printf("¡Felicidades, lograste llegar a la ciudad refugio!\n\n");
        juego->estadoPartida = 1;
        break;
    case 'P':
        printf("¡Obtuviste un punto!\n\n");
        j->puntos++;
        c->componente = '.';
        break;
    case 'V':
        printf("¡Obtuviste una vida extra!\n\n");
        j->cantVidas++;
        c->componente = '.';
        break;
    case 'O':
        printf("Estás en un oasis ¡tenés inmunidad para el siguiente turno!\n\n");
        j->protegido = 1;
        break;
    case 'T':
        printf("Estás en una tormenta, perdés el próximo turno.\n\n");
        if (!j->protegido)
            j->pierdeTurno = 1;
        break;
    case '.':
        printf("El casillero está vacío, no se aplica ningún efecto.\n\n");
        break;
    case 'I':
        printf("Volviste al inicio.\n\n");
        break;
    }


    if (c->cantBandidos > 0 && !(j->protegido))
    {
        printf("¡Te atrapó un bandido! Perdés una vida y volvés al inicio.\n\n");
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
            printf("Te quedaste sin vidas.\n\n");
            printf("GAME OVER.\n\n");
            juego->estadoPartida = -1; // perdio
        }
    }
}

void turno(tJugador *j, tJuego *juego)
{
    int pasos, direccion;

    if (j->pierdeTurno)
    {
        printf("\n\nLos bandidos se mueven mientras estas aturdido!\n");
        for(int i=0; i<3; i++)
        {
            printf(".\n");
        }
        printf("\n");
        j->pierdeTurno = 0;
        encolarMovimientosBandidos(&juego->colaMovimientos, juego, j);
        procesarCola(&juego->colaMovimientos, j, juego);
        mostrarListaDeIzqADer(&juego->tablero, mostrarCasillero);

        return;
    }

    if (j->protegido)
        j->protegido = 0;

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

    juego->posAnteriorJugador = j->posActual; // guardo la posicion donde quedó el jugador al terminar el turno, asi los bandidos la usan para moverse

    mostrarListaDeIzqADer(&juego->tablero, mostrarCasillero);
}

void mostrarCasillero(const void *a)
{
    tCasillero casillero = *(const tCasillero *)a;

    // 1. Agregamos la impresión de la posición al inicio
    // El "%02d" asegura que los números del 1 al 9 tengan un cero adelante (01, 02...)
    printf("%02d: ", casillero.posicion);

    // 2. El resto de tu lógica queda intacta
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

void encolarMovimientosBandidos(tCola* cola, tJuego* juego, tJugador* jugador)
{
    int i;
    for(i=0; i<juego->cantBandidosActivos; i++)
    {
        if(juego->vecBandidos[i].vivo)
        {
            int posB = tirarDado();

            tCasillero* casBandido = (tCasillero*)juego->vecBandidos[i].posActual->dato;
            tCasillero* casJugadorAnterior = (tCasillero*)juego->posAnteriorJugador->dato;

            tMovimiento mov;
            mov.movimientoDe = 'B';
            mov.idBandido = juego->vecBandidos[i].id;
            mov.direccion = (casJugadorAnterior->posicion > casBandido->posicion) ? 1 : 2;
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
