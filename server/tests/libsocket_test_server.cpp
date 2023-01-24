#include <iostream>
#include "../libsockets/sockets.hpp"

int main(int argc, char** argv)
{
    sockets::SocketServer server(4000);
    int client1 = server.wait_for_connection();
    std::cout <<  server.read_chunk(client1) << std::endl;
    std::cout <<  server.read_socket(client1) << std::endl;

    return 0;
}
