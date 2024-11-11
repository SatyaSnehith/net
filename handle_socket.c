#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include "handle_socket.h"
#include <stdatomic.h>

_Atomic int closed = 0;

int check_closed(char* data) {
    if (strcmp(data, "close") == 0) {
        printf("closing\n");
        closed = 1;
        return 1;
    }
    return 0;
}

void* read_socket(void* arg) {
    int socket = *(int*)arg;
    while(!closed) {
        char buffer[1024] = { 0 };
        int valread = read(socket, buffer, 1024 - 1);
        if (valread <= 0) {
            printf("SOCKET CLOSED -> cause: %d", valread);
            break;
        }
        printf("<: %s\n", buffer);
        if (check_closed(buffer)) {
            break;
        }
        memset(buffer, 0, sizeof buffer);
    }
}

void m_handle_socket(int socket) {
    char hello[1024] = { 0 };
    closed = 0;
    pthread_t thread;
    pthread_create(&thread, NULL, read_socket, &socket);
    while(!closed) {
        scanf("%s", hello);
        send(socket, hello, strlen(hello), 0);
        if (check_closed(hello)) {
            break;
        }
    }
}