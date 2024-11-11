// Server side C program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "handle_socket.h"

#define PORT 8080

void handle_socket(int);


int main(int argc, char const* argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);



    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        if ((new_socket = 
                accept(server_fd, (struct sockaddr*)&address, &addrlen)
                ) < 0
            ) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        m_handle_socket(new_socket);

        // closing the connected socket
        close(new_socket);
    }

    // closing the listening socket
    close(server_fd);
    return 0;
}

void s_handle_socket(int socket) {
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";
    while(1) {
        ssize_t valread = read(socket, buffer,
                    1024 - 1); // subtract 1 for the null
                                // terminator at the end
        printf("%s\n", buffer);
        send(socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
        if (strcmp(buffer, "close") == 0) {
            printf("closing\n");
            break;
        }
        memset(buffer, 0, sizeof buffer);
    }
    
}