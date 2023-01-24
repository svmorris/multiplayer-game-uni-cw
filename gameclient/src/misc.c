#include "common.h"


void game_end()
{
    int enemy_pizzas = stage.pizzaTotal - stage.pizzaFound;
    if (stage.pizzaFound > enemy_pizzas)
    {
        printf("You win!!");
    }
    else
    {
        printf("You loose :(");
    }

    exit(0);
}

