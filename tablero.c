#include "tablero.h"
#include "TDA_ListaDobleCircular.h"


void inicializarJuego(tJuego *juego, int cantPos)
{
    crearListaDoble(&juego->tablero);
    crearTablero(&juego->tablero, cantPos);

    juego->nodoInicio = juego->tablero->sig;
    juego->nodoSalida = juego->tablero;
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
    tNodo *inicio = juego->tablero->sig;
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
    return rand() % 6 + 1;
}


int pasosHastaNodo(tNodo *origen, tNodo *destino, char direccion)
{
    int pasos = 0;
    tNodo *actual = origen;
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

void moverJugadorConRebote(tJugador *j, int pasos, char direccion, tJuego *juego)
{
    tNodo *limite;
    int pasosHastaLimite;
    int sobrantes;
    char direccionRebote;

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

void moverJugador(tJugador *j, int pasos, char direccion)
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

void mostrarCasillero(const void *a)
{
    const tCasillero casillero = *(const tCasillero *)a;

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
