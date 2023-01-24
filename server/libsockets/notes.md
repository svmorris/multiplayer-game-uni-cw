# functionality


the server in its normal state will listen

loop:
    on the first connection it will tell the client to wait
    when it recieves a second connection it starts the game

    On game start the server sends the map to the client, along with their position.
    each player will start on opposing ends of the map

    The server uses some sort of struct to pair active connections to each other so it can keep track of them

    The server acts as a relay for each clients position updates
    (if I have time and energy, ill make pizza collections happen on the server. This would be done by checking if player position updates match pizza positions)

The server will connect every x%2=0 connection with the next connection following it.
on second thoughts probably this will only take 2 connections at once for the time being







## Security
#### cheating
The game is controlled on each client. It should be dead simple to cheat in the game if a player uses a modified client.
I am fully aware of this, and dont particularly care as I have 6 days to implement this game

Maybe if I have some time then ill add some sort of error checking to this.





## more details

#### GameServer class
in the constructor this will wait for two connections maybe?
