// CES-35: Redes de Computadores e Internet
// Prof. Cecília de Azevedo Castro Cesar
// Lab 2 - Soquetes
// Written by Lucas do Vale Bezerra e Igor Amâncio Machado Dias, COMP-22

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "parseURL.cpp"

#include <string> // para que reconheca o identificador string 
#include <iostream>
#include <ctime>
#include <fstream>

#define SERVER_PORT 8080  /* arbitrary, but client & server must agree*/
#define BUF_SIZE 4096  /* block transfer size */
#define QUEUE_SIZE 10

using namespace std;

int main(int argc, char *argv[])
{ 
   int s, b, l, fd, sa, on = 1;
   char buf[BUF_SIZE];  /* buffer for outgoing file */
   struct sockaddr_in channel;  /* holds IP address */

   /* Build address structure to bind to socket. */
   memset(&channel, 0, sizeof(channel));
   /* zero channel */
   channel.sin_family = AF_INET;
   channel.sin_addr.s_addr = htonl(INADDR_ANY);
   channel.sin_port = htons(SERVER_PORT);

   /* Passive open. Wait for connection. */
   s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* create socket */
   if (s < 0) {
      printf("socket call failed"); 
      exit(-1);
   }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
   
   b = bind(s, (struct sockaddr *) &channel, sizeof(channel));
   if (b < 0) {
      printf("bind failed"); 
      exit(-1);
   }
   
   l = listen(s, QUEUE_SIZE); /* specify queue size */
   if (l < 0) {
      printf("listen failed"); 
      exit(-1);
   }

   /* Socket is now set up and bound. Wait for connection and process it. */
   while (1) {
      cout << "Servidor inicializado \n\n";
      sa = accept(s, 0, 0); /* block for connection request */
      if (sa < 0) {
         printf("accept failed"); 
         exit(-1);
      }

      string body;
      string header;
      string situ;
      char * dt;
      int body_bytes = 0;
      int resp_bytes = 0;
      int bytes = 0;
      int i;

      read(sa, buf, BUF_SIZE); /* read GET from socket */

      /************** INI PARSING **************/
      char regEx[] = " :\r\n/";
      char *breaks[99];

      cout << "\n" << buf;

      int size = parsing_URL(buf, breaks, regEx);

      string type = breaks[0];
      string objeto = breaks[1];
      string version = breaks[2];
      string hostname;
      string port;
      version = version + "/" + breaks[3];

      for (i = 4; i <= 99; i++){
         string cand = breaks[i];
         if (cand.compare("Host") == 0){
            break;
         }
      }
      
      if (i != 99){
         hostname = breaks[i+1];
         port = breaks[i+2];
      }

      printf("type: %s\n", &type[0]);
      printf("objeto: %s \n", &objeto[0]);
      printf("version: %s \n", &version[0]);
      printf("hostname: %s \n", &hostname[0]);
      printf("port: %s \n", &port[0]);
      /************** FIM PARSING **************/
      
      if (type.compare("GET") != 0 || version.compare("HTTP/1.1") != 0){
         situ = "400 Bad Request";
         body = "<!DOCTYPE HTML PUBLIC HTML 1.1//EN> \n"
               "<html><head>\n"
               "<title>400 Bad Request</title>\n" 
               "</head><body>\n"
               "<h1>Bad Request</h1>\n"
               "<p>Your browser sent a request that this server could not understand.<br />\n"
               "</p>\n"
               "</body></html>";
      }

      /* Get and return the file. */
      fd = open(&objeto[0], O_RDONLY); /* open the file to be sent back */
      if (fd < 0) {
         situ = "404 Not Found";
         body = "<!DOCTYPE HTML PUBLIC HTML 1.1//EN>\n"
               "<html><head>\n" 
               "<title>404 Not Found</title>\n"
               "</head><body>\n"
               "<h1>Not Found</h1>\n" 
               "<p>The requested URL" + objeto + "was not found on this server.</p>\n"
               "</body></html>";
      }
      else {
         situ = "200 OK";
         ifstream in_file(&objeto[0], ios::binary);
         in_file.seekg(0, ios::end);
         bytes = in_file.tellg();
      }

      cout << "\nSituação atual: " << situ << "\n";

      /* Configurando tempo */
      time_t now = time(0);
      tm *gmtm = gmtime(&now);
      dt = asctime(gmtm);
      
      char regEx_aux[] = " \n";
      parsing_URL(dt, breaks, regEx_aux);

      /* Consolidando o Header */
      // ajeitar comprimento
      header = version + " " + situ + "\r\n" + 
               breaks[0] + ", " + breaks[2] + " " + breaks[1] + " " + breaks[4] + " " + breaks[3] + " GMT \r\n" + 
               "Host: " + hostname + "\r\n" + 
               "Content Length: " + to_string(bytes) + "\r\n\r\n";
      
      write(sa, &header[0], header.size());

      /* Coletando e colocando informações para o soquete */
      if (situ.compare("200 OK") == 0){
         while (1) {
            bytes = read(fd, buf, BUF_SIZE); /* read from file */
            if (bytes <= 0) {
               break;  /* check for end of file */
            }
            write(sa, buf, bytes);  /* write bytes to socket */
         }
      }
      else {
         write(sa, &body[0], body.size());  /* write bytes to socket */
      }
      
      close(fd); /* close file */
      close(sa); /* close connection */
   }   
}