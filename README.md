# CPP Competitor

CPP Competetior is a mini program that's let you battle code bots. To get it running you only need
* A class for controlling the game's state
* A class representing each player's action
* An interface representing whatever the player can do

## Tic tac toe

There's a sample implementation of TicTaeToe included.
To write your own bot you need to implement a class inheriting
from `ITicTaeToeClient` and implement the following 3 methods:

```cpp
// Tells your bot whether it's going first.
// This also notifies your bot that a new game will begin now.
virtual void SetIsGoingFirst(bool goesFirst)
{
    ITicTaeToeClient::SetIsGoingFirst(goesFirst);
    //
}
// Returns the name of your bot.
virtual std::string GetName() const = 0;
// This is the core method of the bot.
// The board arguments lets you inspect the current state of the board and
// it's your job tell the game what you want to play.
virtual TicTaeToeTurn NextTurn(const TicTaeToeBoard& board) = 0;
```