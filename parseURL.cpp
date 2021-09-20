// CES-35: Redes de Computadores e Internet
// Prof. Cecília de Azevedo Castro Cesar
// Lab 2 - Soquetes
// Written by Lucas do Vale Bezerra e Igor Amâncio Machado Dias, COMP-22

#include "parseURL.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int parsing_URL(char* URL, char *breaks[], char regEx[])
{
    char *parte;
    parte = strtok(URL, regEx);
    int i = 0;
    while (parte != NULL)
    {
        breaks[i] = parte;
        parte = strtok(NULL, regEx);
        i++;
    }
    return i;
}
