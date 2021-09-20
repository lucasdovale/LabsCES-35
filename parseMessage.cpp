// CES-35: Redes de Computadores e Internet
// Prof. Cecília de Azevedo Castro Cesar
// Lab 2 - Soquetes
// Written by Lucas do Vale Bezerra e Igor Amâncio Machado Dias, COMP-22

#include "parseMessage.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int parsing_message(char* message, char *breaks[], char regEx[]) {
    char *parte;
    parte = strtok(message, regEx);
    int i = 0;
    while (parte != NULL)
    {
        breaks[i] = parte;
        parte = strtok(NULL, regEx);
        i++;
    }
    return i;
}

int start_body(int size, char *breaks[]) {
    int i;
    for(i = 1; i < size; i++) {
        if (strstr(breaks[i], "\r") == NULL)
            return i;

    }
    return -1;
}
