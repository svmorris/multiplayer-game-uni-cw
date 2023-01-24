/*
 * This module is a header only socket implementation.
 *
 * I originally intended to use my libsockets/sockets.cpp 
 * file from the server, however I could not get the game
 * to build as c++. Instead this file has the important
 * functions from that re-implemented in c.
 */
# pragma once

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>



typedef struct
{
    int fd;
    int connection;
    int socket_port;
    struct sockaddr_in address;
} Server;



int write_socket(char* message);
void connect_to_server(char *remote_address);
int read_chunk(char *buffer, int buffer_size);
