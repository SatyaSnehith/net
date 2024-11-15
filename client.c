// Client side C program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "handle_socket.h"
#define PORT 8080

int main(int argc, char const* argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(
            client_fd,
            (struct sockaddr*)&serv_addr,
            sizeof(serv_addr)
        )) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    m_handle_socket(client_fd);

    // closing the connected socket
    close(client_fd);
    return 0;
}

void c_handle_socket(int socket) {
    char hello[1024] = { 0 };
    char buffer[1024] = { 0 };
    while(1) {
        printf("Enter message: ");
        scanf("%s", hello);
        send(socket, hello, strlen(hello), 0);
        int valread = read(socket, buffer,
                    1024 - 1); // subtract 1 for the null
                                // terminator at the end
        printf("%s\n", buffer);
        if (strcmp(hello, "close") == 0) {
            printf("closing\n");
            close(socket);
            break;
        }
        memset(buffer, 0, sizeof buffer);
    }
}