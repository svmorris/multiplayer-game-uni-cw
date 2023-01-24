#include <iostream>
#include "../libsockets/sockets.hpp"

int main(int argc, char** argv)
{
    sockets::SocketClient server(4000);
    server.write_socket("Testing single line send\n");
    server.write_socket("Testing multi-\nline send?");
}
