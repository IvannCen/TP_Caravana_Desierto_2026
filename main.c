#include "configuracion.h"
#include "cola.h"

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

    //para probar lo de los movimientos en la cola

    tCola colaEventos;
    crearCola(&colaEventos);

    //pruebo simulando que el jugador tiro el dado y va para adelante
    tMovimiento movJugador={'J',3,'F'};
    //ahora simulo que la maquina movio al bandido
    tMovimiento movBandido={'B',2,'B'};

    //ahora lo acolo
    aColar(&colaEventos,&movJugador,sizeof(tMovimiento));
    aColar(&colaEventos,&movBandido,sizeof(tMovimiento));

    //desencolo y proceso lo que paso
    tMovimiento movProcesado;
    while(sacarDeCola(&colaEventos,&movProcesado,sizeof(tMovimiento)))
    {
        if(movProcesado.entidad == 'J')
            printf("Resolviendo: El Jugador se mueve %d casillas hacia %c.\n", movProcesado.casillas, movProcesado.sentido);
        else if(movProcesado.entidad == 'B')
            printf("Resolviendo: Un Bandido se mueve %d casillas hacia %c.\n", movProcesado.casillas, movProcesado.sentido);
    }
    printf("Cola vaciada correctamente.\n");
    return 0;
}
