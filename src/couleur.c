#include "couleur.h"

void print_rouge(void)
{
    printf("\033[1;31m");
}

void print_vert(void)
{
    printf("\033[1;32m");
}

void print_jaune(void)
{
    printf("\033[1;33m");
}

void print_bleu(void)
{
    printf("\033[1;34m");
}

void print_magenta(void)
{
    printf("\033[1;35m");
}

void print_cyan(void)
{
    printf("\033[1;36m");
}

void print_blanc(void)
{
    printf("\033[1;37m");
}

void print_orange(void)
{
    printf("\033[38;5;202m");
}

void print_reset(void)
{
    printf("\033[0m");
}