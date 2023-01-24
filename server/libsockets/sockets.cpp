#include "sockets.hpp"

//----------------------------------------------------------------- Base Socket --------------------------------------------------------------

/*
 * Constructor of the BaseSocket class
 */
sockets::BaseSocket::BaseSocket(int port)
{
    // Define address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    // set the port instance variable
    // for future reference
    socket_port = port;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    test_connection(sock);

}

/*
 * Destructor of the base socket
 */
sockets::BaseSocket::~BaseSocket()
{
    std::cout << "Destroying socket: " << sock << std::endl;
}



/*
 * BaseSocket getters and setters
 *
 * Functions to get and set private methods
 * of the BaseSocket class.
 */
int sockets::BaseSocket::get_sock() { return sock; }
int sockets::BaseSocket::get_port() { return socket_port; }
int sockets::BaseSocket::get_connection() { return connection; }
struct sockaddr_in sockets::BaseSocket::get_address() { return address; }
void sockets::BaseSocket::set_connection(int _connection) { connection = _connection; }


/*
 * Function tests if a connection was successful,
 * and prints error if not.
 *
 * This is re-used a lot, so it makes sense to make it
 * its own function.
 */
void sockets::BaseSocket::test_connection(int _connection)
{
    if (_connection < 0)
    {
        perror("Failed to establish connection.");
        exit(EXIT_FAILURE);
    }
}



/*
 *  Function reads one "chunk" from the socket.
 *
 *  A chunk is defined as either 1024 bytes or
 *  until the first newline or null byte.
 */
std::string sockets::BaseSocket::read_chunk(int client_fd)
{
    int bytes_read = 0;
    char buffer[READ_BUFFER_SIZE+1] = "";

    bytes_read = recv(client_fd , buffer , READ_BUFFER_SIZE , 0);
    if (bytes_read < 0)
    {
        perror ("An error occurred while reading from socket");
        return "";
    }

    std::string read_data = buffer;
    memset(buffer, 0, READ_BUFFER_SIZE);
    return read_data;
}

/*
 * Function reads all data from a socket
 * until its closed.
 */
std::string sockets::BaseSocket::read_socket(int client_fd)
{
    int bytes_read = 0;
    std::string read_data = "";
    char buffer[READ_BUFFER_SIZE+1] = "";

    // Read in a loop until the socket closes
    // Re-writing the code here and not calling read_chunk
    // is slightly more performant, and allows for better control.
    while((bytes_read = recv(client_fd , buffer , READ_BUFFER_SIZE , 0)) > 0 )
    {
        if (bytes_read < 0)
        {
            perror ("An error occurred while reading from socket");
            return read_data;
        }
        read_data.append(buffer);
        memset(buffer, 0, READ_BUFFER_SIZE);
    }
    return read_data;
}


int sockets::BaseSocket::write_socket(int client_fd, std::string message)
{

    // The send function returns the amount of bytes that it sent.
    // Using an if statement to make sure all bytes were sent.
    if (send(client_fd, message.c_str(), strlen(message.c_str()), 0) != (long int)strlen(message.c_str()))
    {
        perror("Failed to send data to client");
        return EIO;
    }

    return 0;
}


//----------------------------------------------------------------- Socket Server --------------------------------------------------------------

/*
 * Constructor of the SocketServer class
 */
sockets::SocketServer::SocketServer(int port): BaseSocket(port)
{
    // Bind socket
    set_connection(connect_to_network(get_sock(), get_address()));
    // Make sure the socket was bound
    test_connection(get_connection());

    // start listening on the socket
    listening = start_listening(get_sock());
    test_connection(listening);
}




/*
 * Connection function of the ServerSocket class.
 *
 * This function binds a port.
 */
int sockets::SocketServer::connect_to_network(int sock, struct sockaddr_in address) 
{
    // force server to reconnect to bound port
    // without this you have to change the port after server crashes
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Error while setting socket options");
        return -1;
    }

    int connection = bind(sock, (struct sockaddr*)&address, sizeof(address));


    return connection;
}

int sockets::SocketServer::start_listening(int sock)
{
    // start listening on the socket
    std::cout << "Started listening on port: " << get_port() << std::endl;
    return listen(sock, max_connections);
}

/*
 * Function waits for a connection.
 * When this connection is received, it
 * returns the file-descriptor of the socket
 */
int sockets::SocketServer::wait_for_connection()
{
    return accept(get_sock(), (struct sockaddr*)NULL, NULL);
}



//----------------------------------------------------------------- Socket Client --------------------------------------------------------------

/*
 * Constructor of the SocketServer class
 */
sockets::SocketClient::SocketClient(int port): BaseSocket(port)
{
    // Connect the socket
    set_connection(connect_to_network(get_sock(), get_address()));
    test_connection(get_connection());
}

/*
 * Connection function of the ClientSocket class.
 */
int sockets::SocketClient::connect_to_network(int sock, struct sockaddr_in address) 
{
    return connect(sock, (struct sockaddr*)&address, sizeof(address));
}



/*
 * The client socket doesn't have direct access
 * to the sock file-descriptor of the server.
 *
 * For this reason it calls the BaseSocket read
 * and write methods through this wrapper.
 */
std::string sockets::SocketClient::read_chunk()
{
    return BaseSocket::read_chunk(get_sock());
}
std::string sockets::SocketClient::read_socket()
{
    return BaseSocket::read_socket(get_sock());
}
int sockets::SocketClient::write_socket(std::string message)
{
    return BaseSocket::write_socket(get_sock(), message);
}
