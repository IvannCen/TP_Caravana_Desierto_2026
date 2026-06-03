#include "tablero.h"
#include "TDA_ListaDobleCircular.h"


void inicializarJuego(tJuego *juego, int cantPos)
{
    crearListaDoble(&juego->tablero);
    crearCola(&juego->colaMovimientos);
    crearTablero(&juego->tablero, cantPos);

    juego->nodoInicio = juego->tablero->sig;
    juego->nodoSalida = juego->tablero;
    juego->estadoPartida = 0;
}

void crearTablero(tListaDobleC *pld, int cantPos)
{
    tCasillero casillero;
    int i;

    for(i = 0; i < cantPos; i++)
    {
        casillero.componente = '.';
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

void ubicarJugador(tJuego *juego, tJugador *j)
{
    tNodoLista *inicio = juego->tablero->sig;
    tCasillero *casillero = (tCasillero *)inicio->dato;
    casillero->hayJugador = 1;
    j->posActual = inicio;
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
        printf("\n");

    }
    while(dir != 1 && dir != 2);

    return dir;
}

int tirarDado()
{
    return (rand() % 6) + 1;
}


int pasosHastaNodo(tNodoLista *origen, tNodoLista *destino, int direccion)
{
    int pasos = 0;
    tNodoLista *actual = origen;
    do
    {
        if (direccion == 1)
            actual = actual->sig;
        else
            actual = actual->ant;
        pasos++;
    }
    while (actual != destino && actual != origen);
    return pasos;
}

void moverJugadorConRebote(tJugador *j, int pasos, int direccion, tJuego *juego)
{
    tNodoLista *limite;
    int pasosHastaLimite;
    int sobrantes;
    int direccionRebote;

    if(direccion == 1)
        limite =  juego->nodoSalida;
    else
        limite = juego->nodoInicio;

    pasosHastaLimite = pasosHastaNodo(j->posActual, limite, direccion);

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
    }
}

void aplicarEfectos(tJugador *j, tJuego *juego)
{
    tCasillero *c = (tCasillero *)j->posActual->dato;

    switch (c->componente)
    {
    case 'S':
        printf("Lograste llegar a la ciudad refugio !!\n");
        juego->estadoPartida = 1;   // victoria
        break;
    case 'P':
        printf("Obtuviste un punto !!\n");
        j->puntos++;
        c->componente = '.';   // el premio desaparece
        break;
    case 'V':
        printf("Obtuviste una vida extra !!\n");
        j->cantVidas++;
        c->componente = '.';    // la vida tambien desaparece
        break;
    case 'O':
        printf("Estas en un oasis lo que te genera proteccion !!\n");
        j->protegido = 1;      // protección para el turno siguiente
        break;
    case 'T':
        printf("Estas en una tormenta lo que te hace perder el proximo turno\n");
        if (!j->protegido)
            j->pierdeTurno = 1;
        break;
    case '.':
        break;
    }

    if (c->cantBandidos > 0 && !(j->protegido))
    {
        printf("Te atrapo un bandido\n");
        (j->cantVidas)--;

        // eliminar solo un bandido del casillero
//        eliminarUnBandidoEnNodo(juego->bandidos, j->posicion);

        c->hayJugador = 0;
        ubicarJugador(juego, j);

        if (j->cantVidas == 0)
        {
            printf("Perdiste te quedaste sin vidas, mas suerte la proxima\n");
            juego->estadoPartida = -1;
        }
    }
}

void turno(tJugador *j, tJuego *juego)
{
    int pasos, direccion;

    if (j->pierdeTurno)
    {
        j->pierdeTurno = 0;
        return;
    }

    if (j->protegido)
        j->protegido = 0;

    printf("\n\nAprete ENTER para tirar el dado virtual...");
    getchar();

    pasos = tirarDado();
    printf("\nEl dado cayo en: %d\n", pasos);

    direccion = pedirDireccion();

    // 1 — encolar movimientos
    ponerEnColarMovimientoJugador(&juego->colaMovimientos, direccion, pasos);
//    encolarMovimientosBandidos(&juego->colaMovimientos, juego->bandidos, j);

    // 2 — procesar cola
    procesarCola(&juego->colaMovimientos, j, juego);

    // 3 — actualizar pantalla
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
    else
    {
        printf("[%c] ", casillero.componente);
    }
}
