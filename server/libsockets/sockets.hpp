# pragma once

// c++ stuff
#include <string>
#include <iostream>

// c stuff
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define READ_BUFFER_SIZE 1024



namespace sockets
{


/*
 * The BaseSocket class holds all code that
 * is common between client and server sockets.
 *
 * This type is incomplete. It cannot be used
 * on its own, and should only ever be used
 * through one of its child classes.
 */
class BaseSocket
{
private:
    struct sockaddr_in address;
    int sock;
    int socket_port;
    int connection;
public:
    BaseSocket(int port);
    ~BaseSocket();
    void test_connection(int _connection);
    // virtual int connect_to_network(int sock, struct sockaddr_in address);
    // read and write sockets
    static std::string read_chunk(int client_fd);
    std::string read_socket(int client_fd);
    int write_socket(int client_fd, std::string message);


    // getters
    struct sockaddr_in get_address();
    int get_sock();
    int get_port();
    int get_connection();
    //setters
    void set_connection(int _connection);
};


class SocketServer: public BaseSocket
{
    private:
        int opt = 1;
        int listening;
        int max_connections = 10;
    public:
        SocketServer(int port);
        int connect_to_network(int sock, struct sockaddr_in address);
        int start_listening(int sock);
        int wait_for_connection();

};


class SocketClient: public BaseSocket
{
    public:
        SocketClient(int port);
        int connect_to_network(int sock, struct sockaddr_in address);

        // read and write sockets
        std::string read_chunk();
        std::string read_socket();
        int write_socket(std::string message);
};


};
