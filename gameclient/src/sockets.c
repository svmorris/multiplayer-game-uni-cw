#include "common.h"
#include "sockets.h"


void connect_to_server(char *remote_address)
{
    // Define address structure
    server->address.sin_family = AF_INET;
    server->address.sin_port = htons(SERVER_PORT);


    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, remote_address, &server->address.sin_addr)<=0)  
    { 
        printf(" Invalid address/ Address not supported "); 
        exit(-1);
    } 
   

    // Create socket
    if ((server->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to create new socket");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (( server->connection = connect(server->fd, (struct sockaddr*)&server->address, sizeof(server->address))) < 0)
    {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }
}



/*
 * Function sends a char buffer to the server as text.
 */
int write_socket(char* message)
{

    // The send function returns the amount of bytes that it sent.
    // Using an if statement to make sure all bytes were sent.
    if (send(server->fd, message, strlen(message), 0) != (long int)strlen(message))
    {
        perror("Failed to send data to client");
        return EIO;
    }

    return 0;
}


/*
 *  Function reads one "chunck" from the socket.
 *
 *  A chunk is defined as either 1024 bytes or
 *  until the first newline or null byte.
 */
int read_chunk(char *buffer, int buffer_size)
{
    int bytes_read;

    bytes_read = recv(server->fd , buffer , buffer_size , 0);
    if (bytes_read < 0)
    {
        perror ("An error occurred while reading from socket");
        return EIO;
    }

    return 0;
}
