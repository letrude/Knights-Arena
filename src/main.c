#include "chevalier.h"
#include "monstre.h"
#include "menu.h"
#include "couleur.h"

#define BUFFER_SIZE 50

int main()
{

    struct timespec now;
    timespec_get(&now, TIME_UTC);
    char strNow[BUFFER_SIZE];
    strftime(strNow, BUFFER_SIZE, "%D %T", gmtime(&now.tv_sec));
    srand(now.tv_nsec);

    menuPrincipal();
    return 0;
}