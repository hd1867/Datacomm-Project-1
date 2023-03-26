#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in server_address;
    char query[BUFFER_SIZE], response[BUFFER_SIZE];
    int bytes_received;

    if(argc != 2) {
        printf("Usage: %s <port number>\n", argv[0]);
        exit(1);
    }

    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_address.sin_port = htons(atoi(argv[1]));

    if(connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Enter query in the format \"add/sub/mul:x,y\": ");
    fgets(query, BUFFER_SIZE, stdin);

    if(send(client_socket, query, strlen(query), 0) == -1) {
        perror("send");
        exit(1);
    }

    bytes_received = recv(client_socket, response, BUFFER_SIZE, 0);
    if(bytes_received == -1) {
        perror("recv");
        exit(1);
    }
    response[bytes_received] = '\0';

    printf("Server response: %s\n", response);

    close(client_socket);

    return 0;
}