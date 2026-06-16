#include "configuracion.h"
#include "TDA_ColaDinamica.h"
#include "TDA_ArbolBinarioBusqueda.h"

int mostrarMenuPrincipal()
{
    int opcion;
        // Limpieza de consola (compatible con Windows)
        system("cls");

        printf("\n========================================\n");
        printf("         CARAVANA DEL DESIERTO          \n");
        printf("========================================\n\n");
        printf("  1. Jugar una nueva partida\n");
        printf("  2. Ver el ranking de jugadores\n");
        printf("  3. Salir del juego\n\n");
        printf("========================================\n");
        printf(" Ingrese una opción: ");

        scanf("%d", &opcion);

        // limpieza de buffer
        fflush(stdin);

    return opcion;
}

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

int cmpRanking(const void *a, const void *b)
{
    tRegistroJugador* r1 = (tRegistroJugador*)a;
    tRegistroJugador* r2 = (tRegistroJugador*)b;
    return r1->estado - r2->estado;
}

void mostrarJugadorRanking(const void *a)
{
    tRegistroJugador* r = (tRegistroJugador*)a;
    printf("Jugador: %-20s | Puntos: %d\n", r->nombre, r->estado);
}

void mostrarRanking()
{
    FILE* archJug = fopen("jugadores.dat", "rb");
    FILE* archPart = fopen("partidas.dat", "rb");

    if(!archJug || !archPart)
    {
        printf("Todavia no hay datos suficientes para armar el ranking.\n");
        if(archJug)
            fclose(archJug);
        if(archPart)
            fclose(archPart);
        return;
    }

    fseek(archJug, 0, SEEK_END);
    long cantJugadores = ftell(archJug) / sizeof(tRegistroJugador);
    rewind(archJug);

    if(cantJugadores == 0)
    {
        printf("Todavia no hay jugadores registrados.\n");
        fclose(archJug);
        fclose(archPart);
        return;
    }

    tRegistroJugador* tabla = (tRegistroJugador*)calloc(cantJugadores, sizeof(tRegistroJugador));
    if(!tabla)
    {
        fclose(archJug);
        fclose(archPart);
        return;
    }

    tRegistroJugador regJ;
    while(fread(&regJ, sizeof(tRegistroJugador), 1, archJug))
    {
        tabla[regJ.idJugador-1].idJugador = regJ.idJugador;
        strcpy(tabla[regJ.idJugador - 1].nombre, regJ.nombre);
        tabla[regJ.idJugador-1].estado = 0;
    }

    fclose(archJug);

    tRegistroPartida regP;
    while(fread(&regP, sizeof(tRegistroPartida), 1, archPart))
        tabla[regP.idJugador - 1].estado += regP.puntosObtenidos;
    fclose(archPart);

    tArbolBinBusq arbolRanking;
    crearArbolBinBusq(&arbolRanking);

    int i;
    for(i = 0; i < cantJugadores; i++)
    {
        if(tabla[i].estado > 0)
            insertarEnArbolBinBusq(&arbolRanking, &tabla[i], sizeof(tRegistroJugador), cmpRanking);
    }

    printf("--------------------------------------------------\n");
    printf("%-20s | %s\n", "JUGADOR", "PUNTOS HISTORICOS");
    printf("--------------------------------------------------\n");

    if(arbolRanking == NULL)
        printf("Ningun jugador ha sumado puntos todavia.\n");
    else
        recorrerArbolInOrdenInverso(&arbolRanking, mostrarJugadorRanking);

    printf("--------------------------------------------------\n");

    vaciarArbolBinBusq(&arbolRanking);
    free(tabla);

//    FILE *archRank = fopen("ranking.txt", "rt");
//    if(!archRank)
//    {
//        printf("Todavia no hay partidas registradas.\n");
//    }
//    else
//    {
//        tRegistroRanking reg;
//        while(fscanf(archRank, "%[^|]|%d\n", reg.nombre, &reg.puntos) == 2)
//        {
//            insertarEnArbolBinBusq(&arbolRanking, &reg, sizeof(tRegistroRanking), cmpRanking);
//        }
//        fclose(archRank);
//
//        recorrerArbolInOrdenInverso(&arbolRanking, mostrarJugadorRanking);
//    }
//
//    vaciarArbolBinBusq(&arbolRanking);
}

int cargarIndiceBinario(tArbolBinBusq* pa, const char* Indice)
{
    FILE* arch = fopen(Indice, "rb");
    if(!arch)
        return ERROR;

    tIndiceJugador reg;
    while(fread(&reg, sizeof(tIndiceJugador),1,arch))
        insertarEnArbolBinBusq(pa,&reg,sizeof(tIndiceJugador), cmpIndiceJugador);
    fclose(arch);
    return TODO_OK;
}

void guardarIndiceEnArchivoAux(const tArbolBinBusq* pa, FILE* arch)
{
    if(!*pa)
        return;

    fwrite((*pa)->dato, (*pa)->tamDato, 1, arch);
    guardarIndiceEnArchivoAux(&(*pa)->izq, arch);
    guardarIndiceEnArchivoAux(&(*pa)->der, arch);
}

int guardarIndiceBinario(const tArbolBinBusq* pa, const char* Indice)
{
    FILE* arch = fopen(Indice, "wb");
    if(!arch)
        return ERROR;

    guardarIndiceEnArchivoAux(pa, arch);
    fclose(arch);
    return TODO_OK;
}

int buscarODarDeAltaJugador(tArbolBinBusq* pa, const char* nombre, const char* Jugadores, int* idJugador, long* posArchivo)
{
    tIndiceJugador indiceBuscado;
    strcpy(indiceBuscado.nombre, nombre);
    if (buscarEnArbolBinBusq(pa, &indiceBuscado, sizeof(tIndiceJugador), cmpIndiceJugador) == TODO_OK)
    {
        *posArchivo = indiceBuscado.posArchivo;
        FILE* arch = fopen(Jugadores, "rb");
        if(arch)
        {
            fseek(arch, *posArchivo, SEEK_SET);
            tRegistroJugador reg;
            fread(&reg, sizeof(tRegistroJugador), 1, arch);
            *idJugador = reg.idJugador;
            fclose(arch);
        }
        return TODO_OK;
    }
    else
    {
        FILE* arch = fopen(Jugadores, "a+b");
        if(!arch)
            return ERROR;

        fseek(arch, 0, SEEK_END);
        long offset = ftell(arch);

        int nuevoId = (offset / sizeof(tRegistroJugador)) + 1;

        tRegistroJugador nuevoReg;
        nuevoReg.idJugador = nuevoId;
        strcpy(nuevoReg.nombre, nombre);
        nuevoReg.estado = 1;

        fwrite(&nuevoReg, sizeof(tRegistroJugador), 1, arch);
        fclose(arch);

        tIndiceJugador nuevoIndice;
        strcpy(nuevoIndice.nombre, nombre);
        nuevoIndice.posArchivo = offset;

        insertarEnArbolBinBusq(pa, &nuevoIndice, sizeof(tIndiceJugador), cmpIndiceJugador);

        *idJugador = nuevoId;
        *posArchivo = offset;

        return TODO_OK;
    }
}

int registrarNuevaPartida(const char* Partidas, int idJugador, int puntos, int movimientos)
{
    FILE* arch = fopen(Partidas, "a+b");
    if(!arch)
        return ERROR;

    fseek(arch,0,SEEK_SET);
    long offset = ftell(arch);
    int idPartida = (offset / sizeof(tRegistroPartida)) + 1;

    tRegistroPartida reg;
    reg.idPartida = idPartida;
    reg.idJugador = idJugador;
    reg.puntosObtenidos = puntos;
    reg.cantMovimientos = movimientos;

    fwrite(&reg, sizeof(tRegistroPartida), 1, arch);
    fclose(arch);

    return TODO_OK;
}

int cmpIndiceJugador(const void* a, const void* b)
{
    tIndiceJugador* i1 = (tIndiceJugador*)a;
    tIndiceJugador* i2 = (tIndiceJugador*)b;
    return strcmp(i1->nombre, i2->nombre);
}
