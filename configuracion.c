#include "configuracion.h"


int cargarConfiguracion(tConfiguracion* config, const char* nombarch)
{
    FILE* arch = fopen(nombarch, "rt");
    if(!arch)
        return ERROR;

    fscanf(arch, "cantidad_posiciones:%d\n", &config->cantidad_posiciones);
    fscanf(arch, "vidas_inicio:%d\n", &config->vidas_inicio);
    fscanf(arch, "maximo_bandidos:%d\n", &config->maximo_bandidos);
    fscanf(arch, "maximo_premios:%d\n", &config->maximo_premios);
    fscanf(arch, "maximo_vidas_extra:%d\n", &config->maximo_vidas_extra);
    fscanf(arch, "maximo_oasis:%d\n", &config->maximo_oasis);
    fscanf(arch, "maximo_tormentas:%d\n", &config->maximo_tormentas);

    fclose(arch);

    return TODO_OK;
}

