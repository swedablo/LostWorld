#include "warrior.h"
#include <iostream>
#include <algorithm>


game::Warrior::Warrior(std::string name, Environment* e) : Human(name, e) {
	initiate_attributes();
	_life = max_hp();
	_stamina = max_stamina();
	_status["stamina"] = &_stamina;
	create_func_map();
}

game::Warrior::Warrior(std::string name, int life, int sta, std::vector<std::string> const& str, Environment* e) : Human(name, e) {
	initiate_attributes();
	_life = life;
	_stamina = sta;
	_status["stamina"] = &_stamina;
	create_func_map();
	for (int i = 4; i < (str.size()-2) ; ++i) {
		this->pick_up(str[i]);
	}

}

void game::Warrior::initiate_attributes() {
	// old stuff
	_attributes["max_hp"] = WARRIOR_MAX_HP;
	_attributes["min_dmg"] = WARRIOR_MIN_DMG;
	_attributes["max_dmg"] = WARRIOR_MAX_DMG;
	_attributes["resilience"] = WARRIOR_RESILIENCE;
//	
	_attributes["capacity"] = WARRIOR_CAPACITY;
	// new stuff
	_attributes["max_stamina"] = WARRIOR_MAX_STAMINA;
	_attributes["stamina_fight"] = WARRIOR_STAMINA_FIGHT;
	_attributes["stamina_walk"] = WARRIOR_STAMINA_WALK;
	_attributes["stamina_rest"] = WARRIOR_STAMINA_REST;
} 


std::string game::Warrior::type() const {
	return "Warrior";
}

int game::Warrior::stamina() const {
	return _stamina;
}

int game::Warrior::max_stamina() const {
	return _attributes.find("max_stamina")->second;
}

int game::Warrior::stamina_fight() const {
	return _attributes.find("stamina_fight")->second;
}
int game::Warrior::stamina_walk() const {
	return _attributes.find("stamina_walk")->second;
}
int game::Warrior::stamina_rest() const {
	return _attributes.find("stamina_rest")->second;
}

std::vector <std::string> game::Warrior::allowed_actions() const {
	return {"go <direction>", "fight <target>", "pick up <item>", "drop <item>",
			"use <item>", "rest", "display items", "display attributes", "display status", 
			"info <direction>", "describe <item>"};
}


bool game::Warrior::attack(Character* target) {
	if (stamina() < stamina_fight()) {
		std::cout << "Too tired to attack! Please rest." << std::endl;
		return false;
	}
	bool attacked = Character::attack(target);
	if (attacked) _stamina -= stamina_fight();
	return attacked;
}

bool game::Warrior::walk(game::Direction dir) {
	_stamina -= stamina_walk();
	return game::Character::walk(dir);
	/*
	Environment* e_new = _at_environment->get_neighbour(dir);
	if (e_new) {
		Environment* e_tmp = _at_environment;
		if (_at_environment->leave(this)) 
			if (e_new->enter(this)) {
				return true;
			}
		_at_environment = e_new;
		std::cerr << "Walked into new environment." << std::endl;
		return true;
	}
	std::cerr << "Invalid environment - did not work. " << std::endl;*/
}

bool game::Warrior::rest(std::string const& input) {
	_stamina = std::min(max_stamina(), stamina() + stamina_rest());
	std::cout << "Resting. Stamina is now " << stamina() << "." << std::endl;
	return true;
}


//Extra Assignment


/*Will substitute the action function later*/
bool game::Warrior::action(std::string const& input) {
	//std::cout << "inside action_map" << std::endl;
	std::vector<std::string> subs = split_string(input);
	std::string key = subs[0];
	FPTR pf;
	try {
			pf = func_map.at(key);
	}catch(std::exception const& e)	{
		std::cout << "Invalid input - I don't understand what you want me to do!" << std::endl;
		return false;		
	}
//	std::cout << "will now call function pointer" << std::endl;
	return (this->*pf)(input);
}


void game::Warrior::create_func_map() {
	func_map["go"] = &game::Warrior::go;
	func_map["fight"] = &game::Warrior::fight;
	func_map["pick"] = &game::Warrior::pick;
	func_map["use"] = &game::Warrior::use;
	func_map["drop"] = &game::Warrior::droping;
	func_map["display"] = &game::Warrior::display;
	func_map["info"] = &game::Warrior::info;
	func_map["describe"] = &game::Warrior::describe;
	func_map["rest"] = &game::Warrior::rest;
	
}


bool game::Warrior::go(std::string const& input) {
	//std::cout << "inside GO functon" << std::endl;
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

bool game::Warrior::fight(std::string const& input) {
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


bool game::Warrior::pick(std::string const& input) {
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

bool game::Warrior::use(std::string const& input) {
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


bool game::Warrior::droping(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
	std::cout << input << "     size of input is: " << subs.size() << std::endl;
	if(subs.size() == 1) {
		std::cout << "Invalid input - Argument too short!" << std::endl;
	}
	else {
		std::cout << "inside else statement" <<std::endl;
		std::string item_name = subs[1];
		for (int i = 2; i < subs.size(); ++i) (item_name += " ") += subs[i];
		if (drop(item_name)) std::cout << "Successfully dropped item" << std::endl;
	}
	return false;
}



bool game::Warrior::display(std::string const& input) {
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

bool game::Warrior::info(std::string const& input) {
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


bool game::Warrior::describe(std::string const& input) {
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

std::string game::Warrior::saveInfo(int x, int y) const {
	std::string info = saveCarriedItems(x,y);
	info += type() + ":" +name() + ":" + to_string(health()) + ":" + to_string(stamina()) + ":" + itemsCarried() + to_string(x) + ":" + to_string(y);
	return info;
}

std::string game::Warrior::to_string(int value ) const {
	std::ostringstream os;
	os << value;
	return os.str();	

}




/*
bool game::Warrior::action(std::string const& input) {
	std::vector<std::string> subs = split_string(input);
	if (subs[0] == "go" && subs.size() > 1) {
		if (stamina() < stamina_walk()) {
			std::cout << "You are too tired to move. Please rest." << std::endl;
			return false;
		}
		try {
			if (walk(subs[1])) {
				std::cout << "Successfully moved into new environment! " << std::endl;
			} 
			else std::cout << "Failed to move into the environment. You lost your turn!" << std::endl;
			return true; 
		} catch (std::invalid_argument e) {
			std::cerr << "Sorry! That is not a valid direction." << std::endl;
		}
	}
	else if (subs[0] == "fight" && subs.size() > 1) {
		std::string char_name = subs[1];
		for (int i = 2; i < subs.size(); ++i) (char_name += " ") += subs[i];
		auto chars = _at_environment->characters();
		if (chars.count(char_name) == 1) {
			return attack(chars[char_name]);
		}
		else std::cout << "Character not found in environment!" << std::endl;
			
	}
	else if (subs.size() > 2 && (subs[0] == "pick") && (subs[1] == "up")) {
			//std::cerr << "trying to pick up item " << std::endl;
		std::string item_name = subs[2];
		for (int i = 3; i < subs.size(); ++i) (item_name += " ") += subs[i];
		if (pick_up(item_name)) std::cout << "Successfully picked up item." << std::endl;

	}
	else if (subs[0] == "drop" && subs.size() > 1) {
		std::string item_name = subs[1];
		for (int i = 2; i < subs.size(); ++i) (item_name += " ") += subs[i];
		if (drop(item_name)) std::cout << "Successfully dropped item." << std::endl;
	}
	
	else if (subs[0] == "use" && subs.size() > 1) {
		std::string item_name = subs[1];
	//	std::cerr << "Items name: " << item_name << std::endl;
		for (int i = 2; i < subs.size(); ++i) {
			item_name += " " + subs[i];
		}
		if (use_item(item_name)) std::cout << "Successfully used this item!" << std::endl;
		else std::cout << "Could not use the item." << std::endl;
	}
	else if (subs[0] == "display" && subs[1] == "items")
	{
		display_items();
	}
	else if (subs[0] == "display" && subs[1] == "attributes") {
		std::cout << "These are my attributes:" << std::endl;
		for (auto it = _attributes.begin(); it != _attributes.end(); ++it) 
			std::cout << it->first << " : " << it -> second << std::endl;
	}
	else if (subs[0] == "display" && subs[1] == "status") {
		std::cout << "These are my current stats: " << std::endl;
		for (auto it = _status.begin(); it != _status.end(); ++it)
			std::cout << it->first << " : " << *((int*)it -> second) << std::endl;
	}
	else if (subs[0] == "info" && subs.size() > 1) {
		try {
			std::cout << _at_environment->get_neighbour(subs[1])->about_environment(this);
		} catch (std::invalid_argument) {
			std::cout << "Invalid direction specified." << std::endl;
		}
	}
	else if (subs[0] == "describe" && subs.size() > 1) {
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
//	else if (subs[0] == "rest") {
//		rest();
//		return true;
//	}
	else {
		std::cout << "Invalid input - I don't understand what you want me to do!" << std::endl;
	}
	return false;
}
*/
