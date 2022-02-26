#include "game.h"
#include <iostream>


int main () {
	std::cout << "Welcome to the Lost World. You are stuck on an island swarming with evil-minded undead creatures." 
	 		 << " Rumor has it there is a hidden portal to a better world somewhere on the island. "
	 		 << "You probably do not want to linger around here, so better go out and look for it!" << std::endl;
	game::Game* g = new game::Game();	// create game.	 

	g->play();


	delete g;

}
