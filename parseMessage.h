// CES-35: Redes de Computadores e Internet
// Prof. Cecília de Azevedo Castro Cesar
// Lab 2 - Soquetes
// Written by Lucas do Vale Bezerra e Igor Amâncio Machado Dias, COMP-22

#ifndef PARSE_MESSAGE_H 
#define PARSE_MESSAGE_H

int parsing_message(char* message, char *breaks[], char regEx[]);
int start_body(int size, char *breaks[]);

#endif
