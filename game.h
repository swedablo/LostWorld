//game.h 

#ifndef game__h
#define game__h

namespace game {
	class Game;
	int const MAP_ROWS = 12;
	int const MAP_COLUMNS = 12;
}

#include "priest.h"
#include "undead.h"
#include "warrior.h"
#include "forest.h"
//#include "healing_potion.h"
#include "priest.h"
#include "graveyard.h"
//#include "chest.h"
#include "character.h"
#include "item.h"
#include "human.h"
#include "sea.h"
#include "container.h"
#include "field.h"
#include "direction.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
//#include <stdexcept> // exception. Out_of_range i coord



class game::Game {
	
	private:
		int const _ROWS;
		int const _COLUMNS;
		struct game_Char {
			Character* character;
			bool played;
			game_Char(Character*, bool); // default construct to bool = false.
		};
		std::vector<game_Char> _characters;
		std::vector<std::vector<Environment*> > _playground;
		std::vector<Item*> _items;		
		Container* _chest; //pointer to chest. 
		std::vector <Item* > chest_items;

		

		struct Coordinate {
			int _row;
			int _col;
			Coordinate(int, int);
			explicit Coordinate();
		};

		void create_map(); // set up playground
		void give_each_elem_neighbours();
		void attach_items(); // set up items and attach to environments.
		void setup_characters();
		bool clear_dead(std::vector<game_Char>::iterator&); //true if played char is dead. make sure iterator is set to the right position. 
		void display_info_at_turn(Character*) const;

		//Updated Version
		bool load_game(std::string);
		bool load_game();
		void create_environments(std::vector<std::string> const&);
		void create_characters(std::vector<std::string> const&);
		void create_loadedcharacters(std::vector<std::string> const&);
	//	void load_game_pick_up(std::vector<std::string> const&);
		void create_items(std::vector<std::string> const&);
		std::vector<std::string> split_getline(std::string const&);
		int stringToint(std::string const&) const;
		void chooseCharacter();
		void setPlayground();
		bool save_game(std::string);
		


	public:
		Game();
		~Game();		
		Coordinate get_playground_index(Environment* const&) const; // DENNA SKA VARA PRIVATE!!

	
		std::map<Direction, Environment*> get_neighbours(Environment* const&) const; // eventuellt vara protected?
	

		bool play(); //return true if won. false if lost.
		//caracter_to_env(Character const&, Environment& from, Environment& to);
};



#endif
