#ifndef CONFIGURACION
#define CONFIGURACION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ERROR 0
#define TODO_OK 1

#define CADENA 100

#define JUGADOR 'R'
#define INICIO 'I'
#define SALIDA 'S'
#define PREMIO 'P'
#define VIDA_EXTRA 'V'
#define OASIS 'O'
#define TORMENTA 'T'
#define BANDIDO 'B'

typedef struct
{
    int cantidad_posiciones;
    int vidas_inicio;
    int maximo_bandidos;
    int maximo_premios;
    int maximo_vidas_extra;
    int maximo_oasis;
    int maximo_tormentas;
}tConfiguracion;

int cargarConfiguracion(tConfiguracion* c, const char* nombArchivo);
char* crearVecPos(size_t tam);
void ubicacionAleatoria(char* vec, int tam, char letra, int cant);
void guardarEscenario(char* vec, int tam, const char* nombArchivo);

#endif // CONFIGURACION
