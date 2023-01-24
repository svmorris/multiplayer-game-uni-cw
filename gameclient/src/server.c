#include "server.h"

// Internal functions
int handle_position_updates(char *server_command);

// player2 textures

/*
 * Function sends the position of the player to
 * the server.
 */
int send_update_position()
{
    char message[101] = "";
    snprintf(message, 100, "position: %f %f", player->x, player->y);
    return write_socket(message);
}


/*
 * Send the mug command to the enemy.
 * If you and the enemy are on the same block
 * then this command should mug them.
 */
int send_mug()
{
    return write_socket("mug: \0");
}


/*
 * When a player successfully mugs another player
 * they send a pizza over. 
 *
 * If the player has enough pizzas then this function
 * sends one back to the opponent
 */
int send_pizza()
{
    if (stage.pizzaFound > 0)
    {
        stage.pizzaFound--;
        return write_socket("pizza: \0");
    }

    return -1;
}


/*
 * Function checks for updates from the server,
 * then dispatches them to the right functions.
 *
 * Currently there is only one command the client
 * accepts, which is to update the other players
 * position.
 */
int get_handle_updates()
{
    char buffer[1025] = "";
    read_chunk(buffer, 1024);

    // Enemy sent an update position command
    if (strncmp(buffer, "position: ", 10) == 0)
    {
        handle_position_updates(buffer);
    }

    // If the enemy player sends a mug signal while they
    // are on the same block, then die.
    else if (strncmp(buffer, "mug: ", 5) == 0)
    {
        player->x = player->y = 0;
        send_pizza();
    }
    
    else if (strncmp(buffer, "pizza: ", 7) == 0)
    {
        stage.pizzaFound++;
    }
    else
    {
        fprintf(stderr, "Unknown argument from server: %s\n", buffer);
        return -1;
    }
    return 0;
}

/*
 * Internal function called from get_handle_updates.
 *
 * This function sets the position of player2 from
 * the `position: %f %f` command.
 */
int handle_position_updates(char *server_command)
{
        double x = 0.0;
        double y = 0.0;
        if (sscanf(server_command, "position: %lf %lf", &x, &y) < 0)
        {
            fprintf(stderr, "Invalid position update from server: %s\n", server_command);
            return -1;
        }

        player2->x = x;
        player2->y = y;
        return 0;
}
