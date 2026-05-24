#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

typedef struct
{
    int numeroPosicion; // Para saber si es el casillero 1, 2, 25, etc.
    char elementoBase;  // Aca va el 'I', 'S', 'P', 'V', 'O', 'T', o '.' (lo que ya generas en vecpos)
    int hayJugador;     // 1 si el jugador esta parado aca, 0 si no
    int cantBandidos;   // Cuantos bandidos hay parados en este casillero (pueden caer varios juntos)
} tCasillero;

#endif // JUEGO_H_INCLUDED
