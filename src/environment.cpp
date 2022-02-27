//environment.cpp 

#include "environment.h"

/*
lab2::Environment::Environment(bool N, bool S, bool E, bool W, bool NE, bool NW, bool SE, bool SW) : _item(), _character(), _direction(N,S,E,W,NE,NW,SE,SW) {

}

lab2::Environment::Direction::Direction(bool n, bool s, bool e, bool w, bool ne, bool nw, bool se, bool sw)
*/

//game::Environment::Environment() : _name("Mysterious name"){}

game::Environment::Environment(std::string str) : _name(str)  {
}

game::Environment::~Environment() {

}

std::string game::Environment::about_environment(Character const* src) const {
	return _name + ". This is a " + this->type() + ". " + this->add_information() + "\n" + this->show_characters() + "\n" + this->show_items();
}




game::Environment* game::Environment::get_neighbour(Direction dir) const {
	
	auto it = _neighbours.find(dir);
	if(it!= _neighbours.end())
		return it->second;
	else 
		return nullptr;
}

bool game::Environment::enter(/*std::shared_ptr<Character>*/ Character* ch){
	if(_characters.count(ch->name()) > 0) {
//		std::cerr << "Environment, function: ENTER. " << ch->name() << "was already in the environment " << this->about_environment(ch) << std::endl;
		return false;
	}
	else {
//		std::cerr << ch->name() << " is entering the Environment " << std::endl;
		_characters[ch->name()] = ch;
		return true;
	}
	
}

void game::Environment::add_character(Character* ch) {
	_characters[ch->name()] = ch;
}

void game::Environment::remove(Character* ch) {
	//std::cerr << ch->name() << " is being removed from environment." << std::endl;
	_characters.erase(ch->name());
}

bool game::Environment::leave(/*std::shared_ptr<Character>*/ Character* pch) {
	if(_characters.count(pch->name())==0) {
//		std::cerr << "Environment, function: LEAVE. " << pch->name() << " didn't exist in the environment" << std::endl;	
		return false;
	}
//	std::cerr << pch->name() << " Now leaves the environment " << std::endl;
	_characters.erase(pch->name());
	return true;
}

std::string game::Environment::name() const  {
	return _name;
}

void game::Environment::add_neighbours(std::map<game::Direction,game::Environment*> map) {
	_neighbours = map;
}

std::map<game::Direction, game::Environment*> game::Environment::show_neighbours() const {
	//std::cerr << "Inside show_neighbours"<< std::endl; 
	return _neighbours;
}

std::vector<game::Item* > const& game::Environment::items() {
	return this->_items;
}


bool game::Environment::remove_item(Item* pitem) {
	for (auto it = _items.begin(); it < _items.end(); ++it) {
		if (*it == pitem) {
			it = _items.erase(it);
			return true;
		}
	}
	return false;
}

bool game::Environment::add_item(Item* pitem) {
	_items.push_back(pitem);
	return true;
}

std::string game::Environment::show_items() const {
	std::string str;
	if(_items.size() == 0)
		return "There are no items in this Environment";
	else if (_items.size() ==1) {
		str = "One item in this this Environment:";
	}
	else {
		str = "The items in this Environment are:";
	}
	for(auto it = _items.begin() ; it!= _items.end(); ++it) {
		str += '\n';
		str += (*it)->name();
	}
	return str;
}


std::string game::Environment::show_characters() const {
	std::string str;
	if(_characters.empty())
		str = "There are no characters in this environment. ";
	else if(_characters.size() == 1) {
		auto it = _characters.begin();
		str = "There is one character in this environment:\n" + it->second->name() + ", " + it->second->type() + ". Health: " + to_string(it->second->health()) + ". " ;
	}
	else{
		str = "The characters in this environment are: \n";
		for (auto it = _characters.begin(); it != _characters.end(); ++it)
		{
			str += /* "(" + std::to_string(i+1) + ") " +*/ it->second->name() + ", " + it->second->type() + ". Health: " + to_string(it->second->health()) + ".\n";
		}
	}
	return str;
}

std::map<std::string, game::Character*> const& game::Environment::characters() const {
	return _characters;
}


std::string game::Environment::to_string(int value) const {
	std::ostringstream os;
	os << value;
	return os.str();	
}




