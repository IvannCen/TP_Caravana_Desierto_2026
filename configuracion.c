#include "configuracion.h"

int cargarConfiguracion(tConfiguracion* c, const char* nombArchivo)
{
    FILE* arch = fopen(nombArchivo,"rt");
    if(!arch)
        return ERROR;

    int caracter, contador=0;

    caracter=fgetc(arch);
    while(caracter != EOF)
    {
        if(caracter == ':')
        {
            switch(contador)
            {
                case 0:
                    fscanf(arch,"%d",&c->cantidad_posiciones);
                    break;
                case 1:
                    fscanf(arch,"%d",&c->vidas_inicio);
                    break;
                case 2:
                    fscanf(arch,"%d",&c->maximo_bandidos);
                    break;
                case 3:
                    fscanf(arch,"%d",&c->maximo_premios);
                    break;
                case 4:
                    fscanf(arch,"%d",&c->maximo_vidas_extra);
                    break;
                case 5:
                    fscanf(arch,"%d",&c->maximo_oasis);
                    break;
                case 6:
                    fscanf(arch,"%d",&c->maximo_tormentas);
                    break;
            }
            contador++;
        }
        caracter=fgetc(arch);
    }

    fclose(arch);

    return TODO_OK;
}

char* crearVecPos(size_t tam)
{
    char* vecpos = (char*)malloc(tam*sizeof(char));
    if(!vecpos)
        exit(1);
    int i=0;
    while(i<tam)
    {
        if(i==0)
            *(vecpos+i)=INICIO;
        else if(i==tam-1)
            *(vecpos+i)=SALIDA;
        else
            *(vecpos+i)='.';
        i++;
    }
    return vecpos;
}

void ubicacionAleatoria(char* vec, int tam, char letra, int cant)
{
    int ubi, i=0;
    while(i<cant)
    {
        ubi=rand()%(tam-2)+1;//(tam-2) para no pisar la salida y el +1 para no pisar el inicio
        if(*(vec+ubi)=='.')
        {
            *(vec+ubi)=letra;
            i++;
        }
    }
}

void guardarEscenario(char* vec, int tam, const char* nombArchivo)
{
    FILE* arch=fopen(nombArchivo,"wt");
    if(!arch)
        exit(2);
    int i=0;
    while(i<tam)
    {
        if(i==0)
            fprintf(arch,"%02d: [I J]\n",i+1);
        else
            fprintf(arch,"%02d: %c\n",i+1,*(vec+i));
        i++;
    }
    fclose(arch);
}
