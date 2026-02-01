#include "http_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#define SERVER_PORT 8080
#define WEB_ROOT_DIR "./www"
#define MAX_PENDING_CONNECTIONS 10

static int g_server_socket = -1;

void shutdown_handler(int signal) {
    (void)signal;  
    printf("\nShutting down server...\n");
    if (g_server_socket != -1) {
        close(g_server_socket);
    }
    exit(0);
}
int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    signal(SIGINT, shutdown_handler);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("ERROR: Failed to create socket");
        exit(EXIT_FAILURE);
    }
    
    g_server_socket = server_fd;
    int opt_enable = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, 
                   &opt_enable, sizeof(opt_enable)) < 0) {
        perror("WARNING: setsockopt failed");
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR: Failed to bind socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAX_PENDING_CONNECTIONS) < 0) {
        perror("ERROR: Failed to listen on socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("═══════════════════════════════════════════════\n");
    printf("   HTTP Server Started Successfully\n");
    printf("═══════════════════════════════════════════════\n");
    printf("Port:        %d\n", SERVER_PORT);
    printf("Web Root:    %s\n", WEB_ROOT_DIR);
    printf("URL:         http://localhost:%d\n", SERVER_PORT);
    printf("═══════════════════════════════════════════════\n\n");
    printf("Press Ctrl+C to stop the server\n\n");
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        
        if (client_fd < 0) {
            perror("WARNING: Failed to accept connection");
            continue;
        }
        
        printf("Client connected\n");
        
        handle_client_connection(client_fd, WEB_ROOT_DIR);
        
        printf("Client disconnected\n\n");
    }
    close(server_fd);
    return 0;
}
