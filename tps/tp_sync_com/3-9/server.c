#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9001

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("\n Error: Socket creation error \n");
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    // Configure socket options, forcefully attaching socket to the port 8080
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*) &opt, (socklen_t) sizeof(opt))) {
        perror("\n Error: setsockopt");
        exit(EXIT_FAILURE);
    }

    // bind socket to the specified IP and port
    if (bind(sock, (struct sockaddr *) &address, sizeof(address)))
    {
        perror("\n Error: Bind \n");
        return -1;
    }

    // listen for connections ("1" means that just one request can be queued)
    if (listen(sock, 10) < 0)
    {
        perror("\n Error: Listen \n");
        return -1;
    }

    // integer to hold client socket.
    struct sockaddr_in client_address;
    int client_address_size = sizeof(client_address);
    printf("Server available for accepting client connections...\n"); 
    int client_socket = accept(sock, (struct sockaddr*) &client_address, (socklen_t*) &client_address_size);
    printf("Client connection received.\n");

    char buffer[1024] = {0};
    char response[1034] = {0};
    // Read username from client socket
    read(client_socket, buffer, 1024);
    printf("Received username: %s\n", buffer);

    // Build customized greeting response for this user
    snprintf(response, sizeof(response), "Hello, %s!", buffer);

    // Send customized greeting response to the user
    send(client_socket, response, strlen(response), 0);
    printf("Response sent.\n");
  
    return 0;
}
