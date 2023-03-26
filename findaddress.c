/* Does the DNS lookup for a hostname and prints the IPv4address.
   First argument is hostname
*/


#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>

int main(int argc, char *argv[]) {
   struct hostent *server;
   unsigned char addr[4];

   if (argc < 2) {
      fprintf(stderr,"usage %s hostname\n", argv[0]);
      return 0;
   }

   server = gethostbyname(argv[1]);


   addr[0] = server->h_addr[0];
   addr[1] = server->h_addr[1];
   addr[2] = server->h_addr[2];
   addr[3] = server->h_addr[3];
   
   printf("IPv4 Address: %u.%u.%u.%u\n", addr[0], addr[1], addr[2], addr[3]);

   return 0;
}

