//game.cpp 

#include "game.h"
#include <algorithm>
#include <iostream>

game::Game::Game() : _ROWS(MAP_ROWS), _COLUMNS(MAP_COLUMNS) {

	load_game();

	give_each_elem_neighbours(); // Gives every environment its neigbours
	
}

game::Game::game_Char::game_Char(Character* ch, bool pl = false) : character(ch), played(pl) {

}

game::Game::Coordinate game::Game::get_playground_index(Environment* const& src) const {
	//std::vector<int> = index
	int row = 0;
	int col = 0;

//	std::cerr << "inside get_playground_index, with: " << src->about_environment() << std::endl;
	for(auto it1 = _playground.begin(); it1 != _playground.end(); ++it1) {
		col = 0;
		for (auto it2 = it1->begin(); it2 != it1->end(); ++it2) {
			if(*it2 == src) {
		//		std::cerr << "FOUNDED source: index: row, col = " << row << ", " << col <<". " <<(**it2).type() << std::endl; //<< std::endl << std::endl;
	 			return Coordinate(row, col);
			}
			++col;
		}
		++row;
	}

	return Coordinate();

}

game::Game::Coordinate::Coordinate(){};

game::Game::Coordinate::Coordinate(int row, int col ) : _row(row), _col(col) {}

std::map<game::Direction, game::Environment*> game::Game::get_neighbours(Environment* const& src) const {

	typename game::Game::Coordinate coord = get_playground_index(src);
	std::map<game::Direction , game::Environment*> map;
	if(coord._row -1 >= 0) {  //ta bort sista

		map[Direction("north")] = _playground[coord._row -1][coord._col];
		if(coord._col - 1 >= 0)
			map[Direction("northwest")] =  _playground[coord._row -1][coord._col-1];
		if(coord._col + 1 <= _COLUMNS-1)
			map[Direction("northeast")] =  _playground[coord._row -1][coord._col+1];
	}
	if(coord._row + 1 <= _ROWS-1) {

		map[Direction("south")] = _playground[coord._row+1][coord._col];

		if(coord._col - 1 >= 0)
			map[Direction("southwest")] =  _playground[coord._row +1][coord._col-1];

		if(coord._col + 1 <= _COLUMNS-1)
			map[Direction("southeast")] =  _playground[coord._row +1][coord._col+1];
	}

	if(coord._col - 1 >= 0)
		map[Direction("west")] = _playground[coord._row][coord._col-1];
	if(coord._col + 1 <= _COLUMNS-1)
		map[Direction("east")] = _playground[coord._row][coord._col+1];

	return map;

}

void game::Game::give_each_elem_neighbours() {
//	std::cerr << "Inside give_each_elem_neigbours" << std::endl;
	for (int i = 0; i < _ROWS; ++i)
	{
		for (int j = 0; j < _COLUMNS; ++j)
		{
//			std::cerr << "i = " << i << ", j = " << j << std::endl;
			_playground[i][j]->add_neighbours(this->get_neighbours(_playground[i][j]));
		}
	}
}

bool game::Game::play() {
	std::cout << "Game is now starting..." << std::endl;
	bool dead = false;
	bool won = false;
	bool quit = false;
	bool save = false;
	auto it = _characters.begin();


	while (!dead && !won && !quit && !save) {
		if (it->played) {
			std::cout << "\n\n--------------------- NEW TURN -------------------\n" << std::endl;
			std::cout << "It's your turn to act, " << it->character->name() << "!" << std::endl;
			display_info_at_turn(it->character);
			std::string action;
			do {
				if (_chest->is_open() == it->character) won = true;
				else {
					std::cout << std::endl <<"What do you want to do ('quit'/'save' to quit/save)? " ;
					std::cin.clear();
					std::cin.sync();
					std::getline(std::cin, action);
					if (action == "quit") quit = true;
					if (action == "save") save = true;
				}
			} while (!quit && !save &&!won && !it->character->action(action));
		}
		else while(!it->character->action());	// undead playing.
		dead = clear_dead(it); // clear away dead characters, set to true if played characters is dead.
		if (!won) {
			++it;
		}
		if (it == _characters.end()) {
			it = _characters.begin();
		}
	}
	if (won) {
		std::cout << "Congratulations "<< _chest->is_open()->name() <<"! You beat the game!" << std::endl;
		return true;
	}
	else if (dead) {
		std::cout << "Sorry! You lost the game." << std::endl;
	}
	else if (quit) {
		std::cout << "Quitting..." << std::endl;
	}
	else if (save) {
		std::string game_name;
		while(game_name.size() <= 4) {
			game_name.clear();
			std::cout << "Save the game as: (minimum 5 characters)\n";
			std::cin >> game_name;
			std::cout << "game_name = " << game_name << std::endl;
			if(game_name.size() > 4) {
//				std::cout << "size bigger than 4" << std::endl;
				if (save_game(game_name)) {
					std::cout << "Game Saved. Quitting..." << std::endl;
				}
				else {
					std::cout << "Unable to save game... Try with a new name" << std::endl;
					game_name.clear();
				}
			}
		}
	}

//	std::getchar(); //To pause the game at exit
	return false; 
}

void game::Game::display_info_at_turn(game::Character* ch) const {
	std::cout << "-= Information about environment =-" << std::endl;
	std::cout << ch ->at_environment()->about_environment(ch) << std::endl << std::endl;
	std::cout << "Possible Directions to walk: " << std::endl;
	std::map<Direction, Environment*> neighbours = (ch)->at_environment()->show_neighbours();//asdasdsadsadasd
	for (auto it1 = neighbours.begin(); it1 != neighbours.end() ; ++it1) {
		std::cout << "Direction: " << it1->first << ", " << it1->second->type() << std::endl; // << ", Information about that Environment: " << it1->second->about_environment(*it) << std::endl;
	}

	std::cout << "Your allowed actions are:" << std::endl;
	std::vector<std::string> val = ch->allowed_actions();
	for (int i = 0; i < val.size(); ++i)		
	{
		std::cout << val[i] << std::endl;
	}
}


bool game::Game::clear_dead(std::vector<game::Game::game_Char>::iterator& it_ch) {
	bool played_is_dead = false;
	for (auto it = _characters.begin(); it != _characters.end();) {
		if (it -> character -> health() == 0) {
			std::cout << it->character->name() << " has died!" << std::endl;
			if (it->played) played_is_dead = true;
			delete it->character;
//			std::cerr << "Deletion worked" << std::endl;
			it = _characters.erase(it);
			if (it != _characters.end()) {
				std::cerr << "New char at it: " << it->character->name() << std::endl;
			}
			else {
//				std::cerr << "That was the last one on the list. " << std::endl;
			}
			if (it <= it_ch) --it_ch;
		}
		else {
			++it;
		}
	}
	return played_is_dead;
}

game::Game::~Game() {
	int nc =0, ne = 0, ni = 0;
	for (auto it = _characters.begin(); it != _characters.end(); ++it)
	{
		++nc;
		delete it->character;
	}
	_characters.clear();
	for (int i = 0; i < _playground.size(); ++i)
	{
		for(auto it = _playground[i].begin(); it != _playground[i].end(); ++it)
		{
			//std::cerr << "deleting environments" << std::endl;
			++ne;
			delete *it;
		}
	}
	_playground.clear();
	for (auto it = _items.begin(); it != _items.end(); ++it) {
		++ni;
		delete *it;
	}

	_items.clear();
}



void game::Game::chooseCharacter() {
	char choice;
	std::string name;
	std::cout << "Which class would you like to play, type 1 for Warrior and 2 for Priest: ";
	std::cin >> choice;
	while (choice != '1' && choice != '2') {
		std::cout << "\n Invalid choice! Please try again.\n";
		std::cout << "Which class would you like to play, type 1 for Warrior and 2 for Priest: ";		
		std::cin >> choice;

	}
	std::cout << "Enter the name of your character: ";
	std::cin.ignore();
	std::getline(std::cin, name);

	if (choice == '1') {
		_characters.push_back(game_Char(new Warrior(name, _playground[7][4]), true));
	}
	else {
		_characters.push_back(game_Char(new Priest(name, _playground[7][4]), true));
	}

}

bool game::Game::save_game(std::string game_name) {	
//	std::cout << "Inside save_game" << std::endl;
	std::ofstream myfile;
	game_name = "../data/" + game_name + ".txt";
	myfile.open(game_name);

	if(myfile.is_open()) {
//		std::cout << "Manage to open file, saving playground" <<std::endl;
		//Saving the playground.
		for(int i = 0; i<_playground[0].size(); ++i) { 
			for(int j = 0; j < _playground.size(); ++j) {
				myfile << "Env:" << _playground[i][j]->type()<< ":" << _playground[i][j]->name() <<":"<< i << ":" << j << "\n";
			}
		}
//		std::cout << "Playground saved!" << std::endl;
		for(int i = 0; i<_playground[0].size(); ++i) { 
			for(int j = 0; j < _playground.size(); ++j) {
				//Saving all the items att that lpayground
//				std::cout << "i = " << i << ", j = " << j << std::endl;
				if(!_playground[i][j]->items().empty()) {
					for(auto it = _playground[i][j]->items().begin(); it != _playground[i][j]->items().end(); ++it) {
//						std::cout << "Saving item = " << (*it)->name() <<std::endl;;
						myfile << (*it)->saveInfo(i,j) << "\n";		
					}
//					std::cout << "items saved" << std::endl;
				}
				//Saving all the characters at that playground
				if(!_playground[i][j]->characters().empty()) {
//					std::cout << "inside characters\n";
					for(auto it = _playground[i][j]->characters().begin(); it != _playground[i][j]->characters().end(); ++it) {
//						std::cout << "Saving " << (it)->second->name() << std::endl;
						myfile << (it)->second->saveInfo(i,j) << "\n";
//						std::cout << "Character saved\n";
					}
				}
			}
		}
		myfile.close();
		return true;
	}
//	myfile.close();
	return false;
}


bool game::Game::load_game() {
	std::string str;
	std::cout << "\n      -= LOST WORLDS =-\n";
	while(str.size() == 0) {
		std::cout << " New Game: Press '1' and enter\n";
		std::cout << "Load Game: Press '2' and enter\n";
		std::cin >> str;
		if(str == "1") {
			return load_game("DefaultStartGame");
		}
		else if (str == "2") {
			std::cout << "Which game do you want to load?\n";
			std::cin >> str;
			return load_game(str);
		}
		else {
			std::cout << "Invalid argument\n";
			str.clear();
		}
	}
	return false;
}


bool game::Game::load_game(std::string load_game) {	

	setPlayground();

	load_game = "../data/" + load_game + ".txt";
	std::string default_game = "../data/DefaultStartGame.txt";
	std::string line;
	std::ifstream myfile(load_game);
	std::vector<std::string> substring;
	//std::cerr << "Trying to open file " <<  load_game <<std::endl;
	if(myfile.is_open()) {
		//std::cerr << "file is opened!" << std::endl;
		while(std::getline(myfile,line)) {
		//	std::cerr << "Manage to read line: \n" << line << std::endl;
			substring = split_getline(line);
		//	std::cerr << "Parsed file is:\n";
		//	for(int i = 0; i < substring.size(); ++i) {
		//		std::cerr << substring[i] << std::endl;
		//	}
			if(substring[0] == "Env"){
				create_environments(substring);
			}
			else if(substring[0] == "Undead"  || substring[0] == "Warrior" || substring[0] == "Priest" )
				if(load_game == default_game) {
					create_characters(substring);
				}	
				else {
					create_loadedcharacters(substring);
				}
			else if(substring[0] == "Item" || substring[0] ==  "Chest_item")
				create_items(substring);
			else {
				std::cout << "Unable to understand syntax in file" << std::endl;
				std::cout << line << std::endl;
				return false;
			}
		}
		if(load_game == default_game)
			chooseCharacter();
		return true;
	}
	else {
		std::cout << "Unable to load game from file. File couldn't be opened" << std::endl;
		return false;
	}

}

int game::Game::stringToint(std::string const& str) const {
	int num;
	std::istringstream convert(str);
	if(!(convert >> num)) {
		return -1;
	}
	return num;
}

void game::Game::create_environments(std::vector<std::string> const& str) {
		int x = stringToint(str[str.size()-2]);
		int y = stringToint(str[str.size()-1]);
		if (x < 0 || y < 0) {
			std::cerr << "Invalid coordinate in create_environments, x = " << x << ", y = " << y << std::endl;
			//return false;
		}
		if(str[1] == "Sea") {
//			std::cerr << "Inside Sea:\nx=" << x << std::endl << "y= "<< y <<std::endl << "Name of Sea is " << str[2] << std::endl;;
			_playground[x][y] = new Sea(str[2]);
//			std::cerr << "Managed to create Sea\n";
		}
		else if (str[1] == "Forest") {
			_playground[x][y] = new Forest(str[2]);
		}
		else if (str[1] == "Field") {
			_playground[x][y] = new Field(str[2]);
		}
		else if (str[1] == "Graveyard") {
			_playground[x][y] = new Graveyard(str[2]);
		}
		else {
			std::cout << "No Environment with that name... What happened?!" << std::endl << "Environment is: " << str[1] << std::endl;
		}
}

void game::Game::create_characters(std::vector<std::string> const& str) {
	int x, y;
	if(str[0] == "Undead") {
		x = stringToint(str[str.size()-2]);
		y = stringToint(str[str.size()-1]);
		if (x < 0 || y < 0) {
			std::cerr << "Invalid coordinate in create_character, x = " << x << ", y = " << y << std::endl;
			//return false;
		}
		_characters.push_back(game_Char(new Undead(str[1], _playground[x][y])));
	}
} 

void game::Game::create_loadedcharacters(std::vector<std::string> const& str) {
//	std::cerr << "GAME: Inside create_loadedcharacters  \n";
	int x, y, life, extra;
	x = stringToint(str[str.size()-2]);
	y = stringToint(str[str.size()-1]);
	life = stringToint(str[2]);
	Character* cptr;

	if (x < 0 || y < 0) {
		std::cerr << "Invalid coordinate in create_character, x = " << x << ", y = " << y << std::endl;
		//return false;
	}
	if(str[0] == "Undead") {	
		_characters.push_back(game_Char(new Undead(str[1],life, _playground[x][y])));
	}
	else if(str[0] == "Priest") {
		extra = stringToint(str[3]);
		_characters.push_back(game_Char(new Priest(str[1],life, extra, str, _playground[x][y]),true));
		//load_game_pick_up(str);
	}
	else if(str[0] == "Warrior") {
		extra = stringToint(str[3]);
		_characters.push_back(game_Char(new Warrior(str[1], life, extra, str, _playground[x][y]),true));
		//load_game_pick_up(str);
	}
} 

void game::Game::create_items(std::vector<std::string> const& str) {
	int x = stringToint(str[str.size()-2]);
	int y = stringToint(str[str.size()-1]);
	int weight = stringToint(str[2]);
	std::vector < std::string > types;
	std::map < std::string, int > attributes;
	if (x < 0 || y < 0) {
		std::cerr << "Invalid coordinate in create_items, x = " << x << ", y = " << y << std::endl;
		//return false;
	}
	for (auto it = str.begin(); it != str.end(); ++it) { //Getting our types and attributes
		if(*it == "Warrior" || *it == "Priest") 
			types.push_back(*it);
		else if(it->find("-") != std::string::npos) //Means that it's an attribute
//			std::cerr << "Attribute: " << it->substr(0,it->find("-")) << ", with the value: " <<stringToint(it->substr(it->find("-")+1)) << std::endl;
			attributes[it->substr(0,it->find("-"))] = stringToint(it->substr(it->find("-")+1));
	}
	if(str[0] == "Chest_item") {
		chest_items.push_back(new Item(weight,str[3],attributes,str[1],types));
		_items.push_back(chest_items.back());
	}
	else if (str[1] == "Chest") {
		int capacity = stringToint(str[3]);
		_chest = new Container(weight,str[4],attributes,str[1],types,capacity,_playground[x][y], chest_items);
		//_chest = new Container(weight, description, attributes, name, types, capacity, _playground[4][6], chest_items);
		_items.push_back(_chest);
		chest_items.clear();
	}

	else { // if it's not a chest_item or the Chest.
		_items.push_back(new Item(weight,str[3],attributes,str[1],types));
		_playground[x][y]->add_item(_items.back());
	}
}


/*
void game::Game::load_game_pick_up(std::vector<std::string> const& str) {
	Character* cptr = _characters.back();
	for (int i = 4; i < (str.size()-2) ; ++i)
		cptr->pick_up(str[i]);
}*/



std::vector<std::string> game::Game::split_getline(std::string const& input){
	std::stringstream ss(input);
	std::string substr;
	std::vector<std::string> subs;
	while (std::getline(ss, substr, ':')) {
        subs.push_back(substr);
    }
	return subs;
}


void game::Game::setPlayground() {
	std::vector<Environment*> rad1(12);
	std::vector<Environment*> rad2(12); // = rad3
	std::vector<Environment*> rad3(12); 
	std::vector<Environment*> rad4(12); // = rad5 = rad6
	std::vector<Environment*> rad5(12);
	std::vector<Environment*> rad6(12);
	std::vector<Environment*> rad7(12);
	std::vector<Environment*> rad8(12);
	std::vector<Environment*> rad9(12);
	std::vector<Environment*> rad10(12);
	std::vector<Environment*> rad11(12);
	std::vector<Environment*> rad12(12);

	_playground.push_back(rad1);
	_playground.push_back(rad2);
	_playground.push_back(rad3);
	_playground.push_back(rad4);
	_playground.push_back(rad5);
	_playground.push_back(rad6);
	_playground.push_back(rad7);
	_playground.push_back(rad8);
	_playground.push_back(rad9);
	_playground.push_back(rad10);
	_playground.push_back(rad11);
	_playground.push_back(rad12);


}

void game::Game::setup_characters() {
	_characters.push_back(game_Char(new Undead("Zombie 1", _playground[1][2])));
	_characters.push_back(game_Char(new Undead("Zombie 2", _playground[1][7])));
	_characters.push_back(game_Char(new Undead("Zombie 3", _playground[2][2])));
	_characters.push_back(game_Char(new Undead("Zombie 4", _playground[5][2])));
	_characters.push_back(game_Char(new Undead("Zombie 5", _playground[5][4])));
	_characters.push_back(game_Char(new Undead("Zombie 6", _playground[6][2])));
	_characters.push_back(game_Char(new Undead("Zombie 7", _playground[7][1])));
	_characters.push_back(game_Char(new Undead("Zombie 8", _playground[7][6])));
	_characters.push_back(game_Char(new Undead("Zombie 9", _playground[10][3])));
	_characters.push_back(game_Char(new Undead("Wight 1", _playground[1][3])));
	_characters.push_back(game_Char(new Undead("Wight 2", _playground[3][5])));
	_characters.push_back(game_Char(new Undead("Wight 3", _playground[6][7])));
	_characters.push_back(game_Char(new Undead("Wight 4", _playground[7][7])));
	_characters.push_back(game_Char(new Undead("Wight 5", _playground[8][4])));
	_characters.push_back(game_Char(new Undead("Ghoul 1", _playground[3][7])));
	_characters.push_back(game_Char(new Undead("Ghoul 2", _playground[5][9])));
	_characters.push_back(game_Char(new Undead("Ghoul 3", _playground[6][10])));
	_characters.push_back(game_Char(new Undead("Ghoul 4", _playground[10][6])));

	char choice;
	std::string name;
	std::cout << "Which class would you like to play, type 1 for Warrior and 2 for Priest: ";
	std::cin >> choice;
	while (choice != '1' && choice != '2') {
		std::cout << "\n Invalid choice! Please try again.\n";
		std::cout << "Which class would you like to play, type 1 for Warrior and 2 for Priest: ";		
		std::cin >> choice;

	}
	std::cout << "Enter the name of your character: ";
	std::cin.ignore();
	std::getline(std::cin, name);

	if (choice == '1') {
		_characters.push_back(game_Char(new Warrior(name, _playground[7][4]), true));
	}
	else {
		_characters.push_back(game_Char(new Priest(name, _playground[7][4]), true));
	}
	_characters.push_back(game_Char(new Undead("Wight 6", _playground[9][4])));
	/*std::cerr << "Characters in game: \n";
	for (auto it = _characters.begin(); it != _characters.end(); ++it)
		std::cerr << it->character->name() << ", " << it->character->type() << std::endl;
		*/


}

void game::Game::attach_items() {
	// Item 1. Warrior.
	int weight = 35; 
	std::string name = "Greatshield";
	std::string description = "A heavy shield in iron. Seems a good fit for the strong.";
	std::vector < std::string > types = {"Warrior"};
	std::map < std::string, int > attributes;
	attributes["resilience"] = 15;
	attributes["stamina_rest"] = 10;
	_items.push_back(new Item(weight, description, attributes, name, types));
	_playground[2][9] -> add_item(_items.back());

	// Item 2. Warrior
	weight = 30;
	name = "Battleaxe";
	description = "An axe held in one hand by the melee fighter. Vicious!";
	attributes.clear();
	attributes["max_dmg"] = 7;
	attributes["min_dmg"] = 3;
	_items.push_back(new Item(weight, description, attributes, name, types));
	_playground[9][4] -> add_item(_items.back());

	// Item 3. Priest and Warrior.
	weight = 20;
	name = "Ancient Cloak";
	description = "This cloak looks like a relic of the past. Decorated in mysterious symbols, anyone carrying this piece would be empowered.";
	types.push_back("Priest");
	attributes.clear();
	attributes["max_hp"] = 20;
	attributes["max_dmg"] = 3;
	attributes["resilience"] = 10;
	_items.push_back(new Item(weight, description, attributes, name, types));
	_playground[9][8] -> add_item(_items.back());

	// Item 4. Priest
	weight = 12;
	name = "Book of Enlightenment";
	description = "For the wise and knowledgeable.";
	types.clear();
	types.push_back("Priest");
	attributes.clear();
	attributes["max_enlightenment"] = 25;
	attributes["meditate_enlightenment"] = 10;
	_items.push_back(new Item(weight, description, attributes, name, types));
	_playground[3][3] -> add_item(_items.back());

	// Item 5. Priest
	weight = 12;
	name = "Staff of the Wise";
	description = "A powerful, magical staff.";
	attributes.clear();
	attributes["max_dmg"] = 6;
	attributes["min_dmg"] = 5;
	attributes["heal_power"] = 5;
	_items.push_back(new Item(weight, description, attributes, name, types));
	_playground[5][8] -> add_item(_items.back());

	// Item 6. The magical key needed for the chest. Does not give attributes. 
	weight = 10;
	name = "Magical Key";
	description = "Hmm, where could this key go? It is quite heavy...";
	attributes.clear();
	types.clear();
	_items.push_back(new Item(weight, description, attributes, name, types));
	_playground[8][9] -> add_item(_items.back());

	// Chest and items in there. 
	std::vector <Item* > chest_items;

	weight = 40;
	name = "Junk and Debris";
	description = "This stuff doesn't seem useful for anything.";
	chest_items.push_back(new Item(weight, description, attributes, name, types));
	_items.push_back(chest_items.back());

	weight = 100;
	name = "Portal to Another World";
	description = "WIN!"; 
	chest_items.push_back(new Item(weight, description, attributes, name, types));
	_items.push_back(chest_items.back());

	weight = 300;
	int capacity = 500;
	name = "Chest";
	description = "What could this chest contain? It is locked, a key is needed to unlock it.";
	_chest = new Container(weight, description, attributes, name, types, capacity, _playground[4][6], chest_items);
	_items.push_back(_chest);
//	_playground[4][6].add_item(new Container(weight, description, attributes, name, types, capacity, chest_items));
	
}

void game::Game::create_map() {
	std::vector<Environment*> rad1;
	std::vector<Environment*> rad2; // = rad3
	std::vector<Environment*> rad3; 
	std::vector<Environment*> rad4; // = rad5 = rad6
	std::vector<Environment*> rad5;
	std::vector<Environment*> rad6;
	std::vector<Environment*> rad7;
	std::vector<Environment*> rad8;
	std::vector<Environment*> rad9;
	std::vector<Environment*> rad10;
	std::vector<Environment*> rad11;
	std::vector<Environment*> rad12;

//rad1
	for (int i = 0; i<12 ; ++i) {
		rad1.push_back(new Sea("Ocean"));
	}
	_playground.push_back(rad1);
//rad2
	rad2.push_back(new Sea("Ocean"));
	for (int i = 0; i<10; ++i) {
		rad2.push_back(new Forest("Sherwood"));
	}
	rad2.push_back(new Sea("Ocean"));
	_playground.push_back(rad2);

//rad3

	rad3.push_back(new Sea("Ocean"));
	for (int i = 0; i<10; ++i) {
		rad3.push_back(new Forest("Sherwood"));
	}
	rad3.push_back(new Sea("Ocean"));
	_playground.push_back(rad3);

//rad 4
	rad4.push_back(new Sea("Ocean"));
	rad4.push_back(new Forest("Sherwood"));
	rad4.push_back(new Forest("Sherwood"));
	for(int i = 0; i<5; ++i) {
		rad4.push_back(new Field("Barrens"));
	}
	for(int i = 0; i<3; ++i) {
		rad4.push_back(new Graveyard("Church"));
	}
	rad4.push_back(new Sea("Ocean"));
	_playground.push_back(rad4);

//rad5
	rad5.push_back(new Sea("Ocean"));
	rad5.push_back(new Forest("Sherwood"));
	rad5.push_back(new Forest("Sherwood"));
	for(int i = 0; i<5; ++i) {
		rad5.push_back(new Field("Barrens"));
	}
	for(int i = 0; i<3; ++i) {
		rad5.push_back(new Graveyard("Church"));
	}
	rad5.push_back(new Sea("Ocean"));
	_playground.push_back(rad5);

//rad6
	rad6.push_back(new Sea("Ocean"));
	rad6.push_back(new Forest("Sherwood"));
	rad6.push_back(new Forest("Sherwood"));
	for(int i = 0; i<5; ++i) {
		rad6.push_back(new Field("Barrens"));
	}
	for(int i = 0; i<3; ++i) {
		rad6.push_back(new Graveyard("Church"));
	}
	rad6.push_back(new Sea("Ocean"));
	_playground.push_back(rad6);

//rad7
	rad7.push_back(new Sea("Ocean"));
	for(int i = 0; i< 10; ++i) {
		rad7.push_back(new Forest("Sherwood"));
	}
	rad7.push_back(new Sea("Ocean"));
	_playground.push_back(rad7);

//rad8
	rad8.push_back(new Sea("Ocean"));
	for (int i = 0; i<4; ++i) {
		rad8.push_back(new Field("Farm"));
	}
	for (int i = 0; i < 2; ++i)
	{
		rad8.push_back(new Graveyard("Tombs"));
	}
	for (int i = 0; i < 4; ++i)
	{
		rad8.push_back(new Field("Plain"));
	}
	rad8.push_back(new Sea("Ocean"));
	_playground.push_back(rad8);

//rad9

	rad9.push_back(new Sea("Ocean"));
	for (int i = 0; i<4; ++i) {
		rad9.push_back(new Field("Farm"));
	}
	for (int i = 0; i < 2; ++i)
	{
		rad9.push_back(new Forest("Amazona"));
	}
	for (int i = 0; i < 4; ++i)
	{
		rad9.push_back(new Field("Plain"));
	}
	rad9.push_back(new Sea("Ocean"));
	_playground.push_back(rad9);

//rad10
	rad10.push_back(new Sea("Ocean"));
	for (int i = 0; i<4; ++i) {
		rad10.push_back(new Field("Farm"));
	}
	for (int i = 0; i < 2; ++i)
	{
		rad10.push_back(new Forest("Amazona"));
	}
	for (int i = 0; i < 4; ++i)
	{
		rad10.push_back(new Field("Plain"));
	}
	rad10.push_back(new Sea("Ocean"));
	_playground.push_back(rad10);

//rad11
	rad11.push_back(new Sea("Ocean"));
	for (int i = 0; i<4; ++i) {
		rad11.push_back(new Field("Farm"));
	}
	for (int i = 0; i < 2; ++i)
	{
		rad11.push_back(new Forest("Amazona"));
	}
	for (int i = 0; i < 4; ++i)
	{
		rad11.push_back(new Field("Plain"));
	}
	rad11.push_back(new Sea("Ocean"));
	_playground.push_back(rad11);

//rad12
	for (int i = 0; i<12 ; ++i) {
		rad12.push_back(new Sea("Ocean"));
	}
	_playground.push_back(rad12);
	
	int ne = 0;
	for (auto it = _playground.begin(); it != _playground.end(); ++it) {
		ne += it->size();
	}
}




