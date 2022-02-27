//priest.cpp 

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <exception>
#include <stdlib.h>
#include "priest.h"

//game::Priest::Priest(std::string name) : game::Human(name) _enlightment(100) {
//}
/*
game::Priest::Priest() {
	initiate_attributes();
	_life = max_hp();
	_enlightenment = max_enlightenment();
	_status["enlightenment"] = &_enlightenment;
}

game::Priest::Priest(std::string name) : Human(name) {
	initiate_attributes();
	_life = max_hp();
	_enlightenment = max_enlightenment();
	_status["enlightenment"] = &_enlightenment;
}
*/
game::Priest::Priest(std::string name, Environment* e) : Human(name, e) {
	initiate_attributes();
	_life = max_hp();
	_enlightenment = max_enlightenment();
	_status["enlightenment"] = &_enlightenment;
	create_func_map();
}

game::Priest::Priest(std::string name, int life, int enl, std::vector<std::string> const& str, Environment* e) : Human(name, e) {
	initiate_attributes();
	_life = life;
	_enlightenment = enl;
	_status["enlightenment"] = &_enlightenment;
	create_func_map();
	if( str.size() >6) {
		std::cerr << "Telling me to pick up items\n";
		for (int i = 4; i < (str.size()-2) ; ++i) {
			std::cerr << "Inside for loop in priest constructor\n";
			this->pick_up(str[i]);
		}
	}
	
}

void game::Priest::initiate_attributes() {
	// old stuff
	_attributes["max_hp"] = PRIEST_MAX_HP;
	_attributes["min_dmg"] = PRIEST_MIN_DMG;
	_attributes["max_dmg"] = PRIEST_MAX_DMG;
	_attributes["resilience"] = PRIEST_RESILIENCE;
//	_attributes["max_stamina"] = PRIEST_MAX_STAMINA;
	_attributes["capacity"] = PRIEST_CAPACITY;
	// new stuff
	_attributes["max_enlightenment"] = PRIEST_MAX_ENLIGHTENMENT;
	_attributes["heal_power"] = PRIEST_HEAL_POWER;
	_attributes["heal_cost"] = PRIEST_HEAL_COST;
	_attributes["attack_cost"] = PRIEST_ATTACK_COST;
	_attributes["meditate_power"] = PRIEST_MEDITATE_POWER;
	_attributes["graveyard_enlightenment"] = PRIEST_GRAVEYARD_ENLIGHTENMENT;
//	_attributes["meditate_power_stamina"] = PRIEST_MEDITATE_POWER_STAMINA;
} 

std::string game::Priest::type() const {
	return "Priest";
}


std::vector <std::string> game::Priest::allowed_actions() const {
	return {"go <direction>", "fight <target>", "pick up <item>", "drop <item>",
			"use <item>" /*, "open <item>"*/, "heal", "meditate", "display items", "display attributes",
			 "display status", "info <direction>", "describe <item>"};
}



// Players turn to do something.
	//get option of different actions. In your environment there are som things on the ground? There are a zombie walking around? Go to another
	//choose an option. If this option is on the std::map, do it. Otherwise say tat you cannot do it and try again.


bool game::Priest::attacks_by_magic() const {
	return true;
}

int game::Priest::enlightenment() const {
	return _enlightenment;
}

int game::Priest::max_enlightenment() const {
	return _attributes.find("max_enlightenment")->second;
}

int game::Priest::heal_power() const {
	return _attributes.find("heal_power")->second;
}
int game::Priest::heal_cost() const {
	return _attributes.find("heal_cost")->second;
}
int game::Priest::attack_cost() const {
	return _attributes.find("attack_cost")->second;
}
int game::Priest::meditate_power() const {
	return _attributes.find("meditate_power")->second;
}



bool game::Priest::attack(Character* target) {
	if (this == target) {
		std::cerr << "you cant attack yourself! \n" << std::endl;
		return false; 
	}

	if (_enlightenment >= attack_cost()) {
		std::cout << this->name() << " is casting God's wrath on target " << target->name() << std::endl;
		//std::cerr << this->name() << " is attacking " << target->name() << std::endl;
		target->taking_damage(generate_damage(), this);	
		_enlightenment -= attack_cost();
		return true;
	}
	std::cout << "Too little enlightenment to attack!" << std::endl; 
	return false;
}

void game::Priest::graveyard_effect() {
	std::cerr << "Feeling more enlightened..." << std::endl;
	_enlightenment = std::min(get_attribute("max_enlightenment"), 
					  _enlightenment + get_attribute("graveyard_enlightenment"));
}



/*Will substitute the action function later*/
bool game::Priest::action(std::string const& input) {
	//std::cout << "inside action_map" << std::endl;
	std::vector<std::string> subs = split_string(input);
	std::string key = subs[0];
	FPTR pf;
	try {
			pf = func_map.at(key);
	}catch(std::exception const& e)	{
		//std::cout << "Argument is: " << key << std::endl;
		std::cout << "Invalid input - I don't understand what you want me to do!" << std::endl;
		return false;		
	}
	return (this->*pf)(input);
}

void game::Priest::create_func_map() {

//	std::cout << "Create function map" << std::endl;
	func_map["go"] = &game::Priest::go;
	func_map["fight"] = &game::Priest::fight;
	func_map["meditate"] = &game::Priest::meditate;
	func_map["heal"] = &game::Priest::heal;
	func_map["pick"] = &game::Priest::pick;
	func_map["use"] = &game::Priest::use;
	func_map["drop"] = &game::Priest::droping;
	func_map["display"] = &game::Priest::display;
	func_map["info"] = &game::Priest::info;
	func_map["describe"] = &game::Priest::describe;
	
}



bool game::Priest::go(std::string const& input) {
//	std::cout << "inside GO functon" << std::endl;
	std::vector<std::string> subs = split_string(input);
	if (subs.size() > 1) {
		try {
			return walk(subs[1]);
		} catch (std::invalid_argument e) {
			std::cerr << "Sorry! That is not a valid direction." << std::endl;
			return false;
		}
	}
	return false;
}

bool game::Priest::fight(std::string const& input) {
//	std::cout << "Inside fight function " << std::endl;
	std::vector<std::string> subs = split_string(input);
	std::string char_name = subs[1];
	for (int i = 2; i < subs.size(); ++i) (char_name += " ") += subs[i];
	auto chars = _at_environment->characters();
	if (chars.count(char_name) == 1) {
		return attack(chars[char_name]);
	}
	else std::cout << "Character not found in environment!" << std::endl;
}

bool game::Priest::meditate(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
	if(subs.size() > 1) {
		std::cout << "Invalid input - I don't understand what you want me to do!" << std::endl;
		return false;
	}
	std::cerr << "Meditating....." << std::endl;
	_enlightenment += std::min(meditate_power(), max_enlightenment() - enlightenment());
	std::cout << "Enlightenment increased to " << enlightenment() << "." << std::endl;
	return true;
}


bool game::Priest::heal(std::string const& input) {	//make bool?
	std::vector<std::string> subs = split_string(input);
	if(subs.size() > 1) {
		std::cout << "Invalid input - I don't understand what you want me to do!" << std::endl;
		return false;
	}
	else if (enlightenment() > heal_cost()) {
		_life += std::min(heal_power(), max_hp() - health());
		_enlightenment -= heal_cost();
		std::cout << "Your life has increased to " << health() <<"!"<< std::endl;
		return true;
	}
	else {
		std::cout << "Insufficient enlightenment to heal" << std::endl;
		return false;
	}
}


bool game::Priest::pick(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
	if((subs.size() <= 2) || (subs[1] != "up") )  {
		std::cout << "Invalid input - I don't understand what you want me to do!" << std::endl;
		return false;
	}
	std::cerr << "trying to pick up item " << std::endl;
	std::string item_name = subs[2];
	for (int i = 3; i < subs.size(); ++i) (item_name += " ") += subs[i];
	if (pick_up(item_name)) std::cout << "Successfully picked up item." << std::endl;
	return false;
}

bool game::Priest::use(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
		if(subs.size() == 1) {
			std::cout << "Invalid input - Argument too short!" << std::endl;
		}
		else {
			std::string item_name = subs[1];
			std::cerr << "Items name: " << item_name << std::endl;
			for (int i = 2; i < subs.size(); ++i) {
				item_name += " " + subs[i];
			}
			if (use_item(item_name)) std::cout << "Successfully used this item!" << std::endl;
			else std::cout << "Could not use the item." << std::endl;
		}
		return false;
}


bool game::Priest::droping(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
	if(subs.size() == 1) {
		std::cout << "Invalid input - Argument too short!" << std::endl;
	}
	else {
		std::string item_name = subs[1];
		for (int i = 2; i < subs.size(); ++i) (item_name += " ") += subs[i];
		if (drop(item_name)) std::cout << "Successfully dropped item" << std::endl;
	}
	return false;
}


bool game::Priest::display(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
	if(subs[1] == "items") {
		display_items();
		
	}
	else if(subs[1] == "attributes") {
		std::cout << "These are my attributes:" << std::endl;
		for (auto it = _attributes.begin(); it != _attributes.end(); ++it) 
			std::cout << it->first << " : " << it -> second << std::endl;
	}
	else if(subs[1] == "status") {
		std::cout << "These are my current stats: " << std::endl;
		for (auto it = _status.begin(); it != _status.end(); ++it)
			std::cout << it->first << " : " << *((int*)it -> second)<< std::endl;
		
	}
	else {
		std::cout << "Invalid input - I don't understand what you want me to do!" << std::endl;
	}
	return false;
}

bool game::Priest::info(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
	if (subs.size()>1) {
		try {
				std::cout << _at_environment->get_neighbour(subs[1])->about_environment(this);
		} 
		catch (std::invalid_argument) {
			std::cout << "Unknown direction!" << std::endl;
		}
	}
	return false;
}


bool game::Priest::describe(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
	if(subs.size()>1){
		std::string item_name = subs[1];
		for (int i = 2; i < subs.size(); ++i) (item_name += " ") += subs[i];
		for (auto it = _carrying.begin(); it != _carrying.end(); ++it) 
		{
			if ((*it)->name() == item_name) {
				(*it)->describe();
				return false;
			}
		}
		for (auto it = _at_environment->items().begin(); it != _at_environment->items().end(); ++it) {
			if ((*it)->name() == item_name) {
				(*it)->describe();
				return false;
			}
		}
		std::cout << "Sorry - item name is invalid." << std::endl;
	}
	else {
		std::cout << "Invalid input - Argument too short!" << std::endl;
	}
	return false;
}

std::string game::Priest::saveInfo(int x, int y) const {
	std::string info = saveCarriedItems(x,y);
	info += type() + ":" +name() + ":" + to_string(health()) + ":" + to_string(enlightenment()) + ":" + itemsCarried() + to_string(x) + ":" + to_string(y);
	return info;
}


std::string game::Priest::to_string(int value ) const {
	std::ostringstream os;
	os << value;
	return os.str();	

}

