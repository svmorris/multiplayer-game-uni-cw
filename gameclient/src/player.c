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

void check_player_collision();
static SDL_Texture *pete[4];

void initPlayer(void)
{
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));
	stage.entityTail->next = player;
	stage.entityTail = player;
    
	player2 = malloc(sizeof(Entity));
	memset(player2, 0, sizeof(Entity));
	stage.entityTail->next = player2;
	stage.entityTail = player2;

	player->health = 1;
	player2->health = 1;

	pete[0] = loadTexture("gfx/pete01.png");
	pete[1] = loadTexture("gfx/pete02.png");
	pete[2] = loadTexture("gfx/pete01_attack.png");
	pete[3] = loadTexture("gfx/pete02_attack.png");

	player->texture = pete[0];
	player2->texture = pete[0];

	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
	SDL_QueryTexture(player2->texture, NULL, NULL, &player2->w, &player2->h);
}

void doPlayer(void)
{
	player->dx = 0;
    player->attack = false;

    if (app.keyboard[SDL_SCANCODE_SPACE])
    {
        player->attack = true;
    }

	if (app.keyboard[SDL_SCANCODE_A])
	{
        if (player->attack)
        {
            player->dx = -PLAYER_MOVE_SPEED/2;

            player->texture = pete[3];
        }
        else
        {
            player->dx = -PLAYER_MOVE_SPEED;

            player->texture = pete[1];
        }
	}

	if (app.keyboard[SDL_SCANCODE_D])
	{
        if (player->attack)
        {
            player->dx = PLAYER_MOVE_SPEED/2;

            player->texture = pete[2];
        }
        else
        {
            player->dx = PLAYER_MOVE_SPEED;

            player->texture = pete[0];
        }
	}

    static int frame = 0;

	if (app.keyboard[SDL_SCANCODE_W] &&  player->isOnGround)
	{
        if (player->attack)
        {
            player->dy = -10;
            player->texture = pete[2];
        }
        else
        {
            player->dy = -20;
            player->texture = pete[0];
        }
		player->riding = NULL;

        frame = 0;
        playSound(SND_JUMP, CH_PLAYER);

	}
    else if (player->dy < 0)
    {
        frame++;
        player->texture = pete[(frame / 10) % 2];
    }

    check_player_collision();
}



/*
 * Function  checks if two players have collided.
 * If they have, and player1 is in attack mode,
 * then it will send the mug command to the other
 * player
 */
void check_player_collision()
{
    if (player2->x > player->x-32 && player->x+32 > player2->x)
    {
        if (player2->y > player->y-32 && player->y+32 > player2->y)
        {
            if (player->attack)
            {
                send_mug();
                puts("collision");
            }
        }
    }
}
