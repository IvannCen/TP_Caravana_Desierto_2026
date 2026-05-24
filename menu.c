#include "menu.h"

void menu()
{
    int opc;

    do
    {
        do
        {
            system("cls");
            printf("=======MENU CARAVANA DEL DESIERTO=======\n\n");
            printf("1. Nueva partida\n");
            printf("2. Ranking\n");
            printf("3. Ver jugadores\n");
            printf("\n0. SALIR\n");
            printf("====================================================");
            printf("\nElige una opcion: ");
            scanf("%d", &opc);
            printf("\n\n");
        }
        while(opc < 0 || opc > 3);

        switch(opc)
        {
        case 1:

            pausar();
            break;

        case 2:

            pausar();
            break;

        case 3:

            pausar();
            break;

        }
    }
    while(opc != 0);
}


void pausar()
{
    printf("\nPresione ENTER para continuar...");
    while(getchar() != '\n');
    getchar();
}
