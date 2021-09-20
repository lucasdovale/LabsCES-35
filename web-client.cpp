// CES-35: Redes de Computadores e Internet
// Prof. Cecília de Azevedo Castro Cesar
// Lab 2 - Soquetes
// Written by Lucas do Vale Bezerra e Igor Amâncio Machado Dias, COMP-22

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string> // para que reconheca o identificador string 
#include <iostream>
#include <ctime>
#include <fstream>

#define SERVER_PORT 8080 /* arbitrary, but client & server must agree */
#define BUFSIZE 4096     /* block transfer size */

using namespace std;

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


int main(int argc, char **argv) {

  /************** INI PARSING **************/
  char *URL = argv[1];
  char regEx[] = ":/";
  char *breaks[99];

  int size = parsing_message(URL, breaks, regEx);

  string protocol = breaks[0];
  string hostname = breaks[1];
  string porta = breaks[2];

  string objeto = "";
  for(int i = 3; i < size; i++)
    objeto = objeto + "/" + breaks[i];

  string nome = breaks[size - 1];
  /************** FIM PARSING **************/










  /************** INI SOQUETE **************/
  int c, s, bytes;
  char buf[BUFSIZE];          /* buffer for incoming file */
  struct hostent *h;          /* info about server */
  struct sockaddr_in channel; /* holds IP address */

  if (argc != 2) {
    printf("Usage: client server-name file-name");
    exit(-1);
  }

  h = gethostbyname(&hostname[0]); /* look up host’s IP address */

  if (!h) {
    printf("gethostbyname failed to locate %s", &hostname[0]);
    exit(-1);
  }

  s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (s < 0) {
    printf("socket call failed");
    exit(-1);
  }

  memset(&channel, 0, sizeof(channel));
  channel.sin_family = AF_INET;
  memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
  channel.sin_port = htons(stoi(porta));
  c = connect(s, (struct sockaddr *)&channel, sizeof(channel));

  if (c < 0) {
    printf("connect failed");
    exit(-1);
  }





  /************** INI GET **************/
  /* Connection is now established. Send file name including 0 byte at end. */
  string msg_HTTP = "GET " + objeto + " HTTP/1.1\r\nHost:" + hostname + "\r\n\r\n";

  write(s, &msg_HTTP[0], msg_HTTP.size() + 1);
  /************** FIM GET **************/






  /* Go get the file and write it to standard output.*/
  string response = "";

  while (1) {
    bytes = read(s, buf, BUFSIZE);  /* read from socket */
    if (bytes <= 0)
      break;                        /* check for end of file */
    write(1, buf, bytes);           /* write to standard output */
    response = response + buf;
  }
  /************** FIM SOQUETE **************/








  /************** INI CREATE ARCHIVE **************/ 
  char *breaks_body[99];
  char regExBody[] = "\n";
  int size_msg = parsing_message(&response[0], breaks_body, regExBody);

  char *breaks_return[99];
  char regExReturn[] = " ";
  int a = parsing_message(&breaks_body[0][0], breaks_return, regExReturn);

  int msgReturn = atoi(breaks_return[1]);

  int startBody = start_body(size_msg, breaks_body);

  string body = "";

  if (msgReturn == 200) {
    if (startBody != -1)
      for(int i = startBody; i < size_msg; i++)
        body = body + breaks_body[i] + "\n";

    FILE *f = fopen(&nome[0], "w");
    fprintf(f, "%s", &body[0]);
    fclose(f);
    exit(1);
  }
  /************** FIM CREATE ARCHIVE **************/

  close(s);
  exit(-1);
}
