/*
Copyright (C) 2015-2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "common.h"

static void capFrameRate(long *then, float *remainder);


void *get_set_p2_position()
{
    while (true)
    {
        send_update_position();
        get_handle_updates();
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Missing first argument for address.\n");
        exit(-1);
    }

    server = malloc(sizeof(Server));
    // Try and connect to the server
    connect_to_server(argv[1]);

    /*
     * Print out anything that the server sends
     * until it sends a string starting with
     * "Game starting.".
     */
    while (true)
    {
        char buffer[1025] = "";
        read_chunk(buffer, 1024);

        if (strlen(buffer) > 1)
            printf("%s\n", buffer);

        if (strncmp(buffer, "Game starting.", 14) == 0)
            break;

        memset(buffer, 0, 1024);
    }



    // setup sdl window and game
	long then;
	float remainder;

	memset(&app, 0, sizeof(App));
	app.textureTail = &app.textureHead;

	initSDL();

	atexit(cleanup);

	initGame();

	initStage();

	then = SDL_GetTicks();

	remainder = 0;


    /*
     * Run new thread to continuously update
     * the players position, and get the player2's
     * position from the server.
     */
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, get_set_p2_position, NULL);

	while (1)
	{
		prepareScene();

		doInput();

		app.delegate.logic();

		app.delegate.draw();

		presentScene();

		capFrameRate(&then, &remainder);
	}

    pthread_join(thread_id, NULL);
	return 0;
}

static void capFrameRate(long *then, float *remainder)
{
	long wait, frameTime;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frameTime = SDL_GetTicks() - *then;

	wait -= frameTime;

	if (wait < 1)
	{
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}
