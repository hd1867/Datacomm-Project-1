#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int resolveMessage(char* msg, char* rsp)
{
    char* func; 
    int op1, op2, res;

    func = strtok(msg, ":");
    op1 = atoi(strtok( NULL , ","));
    op2 = atoi(strtok( NULL , ","));

    if( !strcmp(func, "add") )
    {
        res = op1 + op2;
        sprintf(rsp, "res:%d", res);
        return 0;
    }

    if( !strcmp(func, "sub") )
    {
        res = op1 - op2;
        sprintf(rsp, "res:%d", res);
        return 0;
    }

    if( !strcmp(func, "mul") )
    {
        res = op1 * op2;
        sprintf(rsp, "res:%d", res);
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char response[1024] = {0};

    if(argc < 2)
    {
        printf("Usage %s <port>\n", argv[0]);
	return 1;
    }


    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Read incoming message from client
    valread = read(new_socket, buffer, 1024);

    // Call resolveMessage function to perform calculation
    resolveMessage(buffer, response);

    // Send response back to client
    send(new_socket, response, strlen(response), 0);

    return 0;
}

