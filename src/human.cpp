//human.cpp 

#include <iostream>
#include <algorithm>
#include "human.h"

/*
game::Human::Human(std::string name) : Character(name)
{
	_attributes["capacity"] = int(0);
}

game::Human::Human() : Character() {
	_attributes["capacity"] = int(0);
}*/

game::Human::Human(std::string name, game::Environment* e) : Character(name, e)
{
	_attributes["capacity"] = int(0);
}

game::Human::~Human() {
	for (auto it = _carrying.begin(); it != _carrying.end(); ++it) 
		_at_environment->add_item(*it);
	_carrying.clear();
	//std::cerr << "Human::destructor." << std::endl;
}
// implementation for item stats something like this. 
void game::Human::update_stats() {
	//std::cerr << "UPDATING STATS!!!" << std::endl;
	initiate_attributes();
	std::map < std::string, int> item_attributes;
	for (auto it = _carrying.begin(); it != _carrying.end(); ++it) {
		if (std::find((*it)->types().begin(), (*it)->types().end(), this->type()) != 
			(*it)->types().end()) 
		{
			item_attributes = (*it)->get_static_attr();
			for (auto itMap = item_attributes.begin(); itMap != item_attributes.end(); ++itMap) 
			{
				if (_attributes.count(itMap->first) == 1)
					_attributes[itMap->first] += itMap->second;
			}

		}	//iterator item allowed types.
		//else std::cerr << "Type not matching - can not benefit from stats." << std::endl;

	}
	// for testing:
	/*for (auto it = _attributes.begin(); it != _attributes.end(); ++it)
		std::cerr << "Attribute " << it->first << " : " << it->second << std::endl;*/

}

int game::Human::capacity() const {
	return _attributes.find("capacity")->second;
}

/*
std::vector<game::Item*> game::Human::carryingItems() const {
	return _carrying;
}*/

/*
int game::Human::stamina() const {
	return _stamina;
}
*/
std::vector<game::Item const*> game::Human::carried_items() const { // this function might not work. 
	std::vector<Item const*> vec;
	for (auto it = _carrying.begin(); it != _carrying.end(); ++it)
		vec.push_back(*it);
	return vec;
}

void game::Human::display_items() const {
	//std::vector < Item const* > item_list = carried_items();
	if (_carrying.empty()) std::cout << "Not carrying any items. " << std::endl; 
	else {
		std::cout << "Carrying items with a total weight of " << carried_weight() << ":" << std::endl;
		for (auto it = _carrying.begin(); it != _carrying.end(); ++it)
			std::cout << (*it)->name() <<", weighing " << (*it)->weight() << std::endl;
	}
}

bool game::Human::has_item(std::string item) const {
	for (auto it = _carrying.begin(); it != _carrying.end(); ++it) {
		if ((*it)->name() == item) return true;
	}
	return false;
}

bool game::Human::use_item(std::string const& item) {
	for(auto it = _at_environment->items().begin(); it != _at_environment->items().end(); ++it ) {
		if ((*it)->name() == item ) {
			if((*it)->use_item(this)) {
				return true;
			}
			return false;
		}
	}
	for (auto it = _carrying.begin(); it != _carrying.end(); ++it) {
		if ((*it)->name() == item ) {
			if((*it)->use_item(this)) {
				return true;
			}
			return false;
		}		
	}
}

bool game::Human::pick_up(std::string const& item) {
	//std::cerr << "trying to pick up " << item <<". Entering for loop:" << std::endl;
	for (auto it = _at_environment->items().begin(); it != _at_environment->items().end(); ++it) {
		//std::cerr << "Item name in for loop atm: " << (*it)->name() << std::endl;
		if ((*it)->name() == item) {
			//std::cerr << "should work to pick up item " << std::endl;
			return pick_up(*it);
		}
	}
	std::cout << "The item, " << item << " does not exist in this environment." << std::endl;
	return false;
}

bool game::Human::pick_up(game::Item* item) {
	if (item->weight() > capacity() - carried_weight())
	{ 
		std::cerr << "Item too heavy. " << std::endl;
		return false;
	}
	else if (_at_environment->remove_item(item)) {
		_carrying.push_back(item);
		update_stats();
		return true;
	}
	return false;
} 

bool game::Human::drop(std::string const& item) {
	for (auto it = _carrying.begin(); it != _carrying.end(); ++it) {
		if ((*it)->name() == item)
			return drop(*it);
	}
	std::cerr << "Could not find item to drop " << std::endl;
	return false;
}

bool game::Human::drop(game::Item* item) {
	auto it = _carrying.begin();
	while (it != _carrying.end()) {
		if (*it == item) {
			_at_environment->add_item(*it);
			_carrying.erase(it);
			update_stats();
			return true;
		}
		++it;
	}
	return false;
}

std::vector <std::string> game::Human::allowed_actions() const {
	return {"go <direction>", "fight <target>", "pick up <item>", "drop <item>",
			"use <item>", "display items"};
}


int game::Human::carried_weight() const {
	int result = 0;
	for (auto it = _carrying.begin(); it != _carrying.end(); ++it)
		result += (*it)->weight();
//	std::cerr << "Carried weight = " << result << std::endl;
	return result;
}

std::string game::Human::itemsCarried() const {
	std::string str = "";
	for(auto it = _carrying.begin(); it != _carrying.end() ; ++it) {
		str += (*it)->name() + ":";
	}
	return str;
}

std::string game::Human::saveCarriedItems(int x, int y) const {
	std::string str = "";
	for(auto it = _carrying.begin() ; it != _carrying.end() ; ++it ) {
		str += (*it)->saveInfo(x,y) + "\n";
	}
	return str;
}


/*
void game::Human::set_name(std::string name) {
	_name = name;
}*/
/*

bool game::Human::taking_damage(int dmg, Character* const& src, std::string attack_type) {
	std::cout << "Ai! You are taking " << dmg <<" damage from" << src->name() << ", " << src->type() << ". " << src->name() << " is "<< attack_type << " you."<<std::endl;
	this->_life -= dmg;
	if(_life > 0) {
		std::cout << "You have " << _life << "left" << std::endl;
		return false; // not dead
	}
	else {
		std::cout << "Joooodeer, that one really got me... You have " << 0 <<" life"<< std::endl;
		return true; // dead;
	}
	

}

*/
