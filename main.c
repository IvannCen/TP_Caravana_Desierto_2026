#include "configuracion.h"

int main()
{
    srand(time(NULL));
    tConfiguracion config;
    if(cargarConfiguracion(&config,"config.txt"))
    {
        printf("%d\n",config.cantidad_posiciones);
        printf("%d\n",config.vidas_inicio);
        printf("%d\n",config.maximo_bandidos);
        printf("%d\n",config.maximo_premios);
        printf("%d\n",config.maximo_vidas_extra);
        printf("%d\n",config.maximo_oasis);
        printf("%d\n\n",config.maximo_tormentas);
    }
    else
    {
        printf("Error en el archivo de configuracion...");
    }

    char* posiciones=crearVecPos(config.cantidad_posiciones);

    for(int i=0;i<config.cantidad_posiciones;i++)
    {
        printf("%02d:%c\n",i+1,*(posiciones+i));
    }

    ubicacionAleatoria(posiciones,config.cantidad_posiciones,BANDIDO,config.maximo_bandidos);
    ubicacionAleatoria(posiciones,config.cantidad_posiciones,PREMIO,config.maximo_premios);
    ubicacionAleatoria(posiciones,config.cantidad_posiciones,VIDA_EXTRA,config.maximo_vidas_extra);
    ubicacionAleatoria(posiciones,config.cantidad_posiciones,OASIS,config.maximo_oasis);
    ubicacionAleatoria(posiciones,config.cantidad_posiciones,TORMENTA,config.maximo_tormentas);

    for(int i=0;i<config.cantidad_posiciones;i++)
    {
        printf("%02d:%c\n",i+1,*(posiciones+i));
    }

    guardarEscenario(posiciones,config.cantidad_posiciones,"caravana.txt");

    free(posiciones);
    return 0;
}
