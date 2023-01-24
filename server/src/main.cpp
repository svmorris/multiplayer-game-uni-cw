#include <stdio.h>
#include "../libsockets/sockets.hpp"
# define GAME_PORT 4532

typedef struct
{
    int player1;
    int player2;
} Game;

void active_game(sockets::SocketServer server, int player1)
{
    // initialise the game
    Game game;

    // Set the first player, and tell it to wait for the second player to join
    game.player1 = player1;
    std::cout << "Client 1 connected at fd: " << game.player1 << std::endl;
    server.write_socket(game.player1, "Waiting for a second player...");

    // wait for a second player
    game.player2 = server.wait_for_connection();
    std::cout << "Client 2 connected at fd: " << game.player2 << std::endl;

    // tell both players that the game can start
    server.write_socket(game.player1, "Game starting.");
    server.write_socket(game.player2, "Game starting.");

 
    int player1_misses = 0;
    int player2_misses = 0;

    // Variables to store the player commands
    std::string player1_command = "";
    std::string player2_command = "";
    while (true)
    {
        player1_command = server.read_chunk(game.player1);
        player2_command = server.read_chunk(game.player2);
        if (player1_command.length() > 1)
        {
            // std::cout << "Sending from " << game.player1 << " to " << game.player2 << " "<< player1_command << std::endl;
            server.write_socket(game.player2, player1_command);
            
            // if the player sends a command then reset their miss counter.
            // player1_misses = 0;
        }
        if (player2_command.length() > 1)
        {
            // std::cout << "Sending from " << game.player1 << " to " << game.player2 << " "<< player1_command << std::endl;
            server.write_socket(game.player1, player2_command);

            // if the player sends a command then reset their miss counter.
            // player2_misses = 0;
        }


        // add one each time the player misses reporting their coordinates
        if (!(player1_command.length() > 1))
            player1_misses++;
        if (!(player2_command.length() > 1))
            player2_misses++;


        // If either player stops responding then end the game
        if (player1_misses  > 100)
        {
            std::cout << "Player 1 has disconnected from fd: " << game.player1 << std::endl;
            return;
        }
        if (player1_misses  > 100)
        {
            std::cout << "Player 2 has disconnected from fd: " << game.player2 << std::endl;
            return;
        }
    }

}

int main(int argc, char **argv)
{
    sockets::SocketServer server(GAME_PORT);

    /*
     * In a loop, at each iteration wait for a connection.
     * when the server got the connection, run the active_game
     * function.
     *
     * If the function returns, then just print game ended,
     * and wait for a new connection.
     */
    while(1)
    {
        active_game(server, server.wait_for_connection());
        std::cout << "Game ended" << std::endl;
    }

    return 0;
}
