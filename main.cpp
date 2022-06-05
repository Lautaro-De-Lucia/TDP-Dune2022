#include <iostream>
#include "player.h"


int main (void){
    Board board(45,25);
    Player new_player(board);
    new_player.run();
}
