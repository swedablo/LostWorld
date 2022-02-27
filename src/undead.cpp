//undead.cpp 

#include "undead.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <stdlib.h>

game::Undead::Undead(std::string str, game::Environment* e) : Character(str, e), _frenzy(false) {
	initiate_attributes();
	_life = max_hp();
	_status["frenzy"] = &_frenzy;
	create_func_map();
}

game::Undead::Undead(std::string str, int life, game::Environment* e) : Character(str, e), _frenzy(false) {
	initiate_attributes();
	_life = life;
	_status["frenzy"] = &_frenzy;
	create_func_map();
}


void game::Undead::update_mode() {
	//std::cerr << "update mode fcn" << std::endl;
	_frenzy = (_at_environment->type() == "Graveyard"
		|| health() <= get_attribute("hp_frenzy"));
}

int game::Undead::generate_damage() const {
	return frenzy() ? 
		max_dmg() : min_dmg() + rand() % (max_dmg() + 1 - min_dmg());
}

void game::Undead::initiate_attributes() {
	// old stuff
	_attributes["max_hp"] = UNDEAD_MAX_HP;
	_attributes["min_dmg"] = UNDEAD_MIN_DMG;
	_attributes["max_dmg"] = UNDEAD_MAX_DMG;

	_attributes["resilience"] = UNDEAD_RESILIENCE;
	// new stuff
	_attributes["resilience_frenzy"] = UNDEAD_RESILIENCE_FRENZY;
	_attributes["hp_frenzy"] = UNDEAD_HP_FRENZY;
	_attributes["hp_graveyard"] = UNDEAD_HP_GRAVEYARD;
} 

std::string game::Undead::type() const{
	return "Undead";
}

bool game::Undead::frenzy() const {
//	std::cerr << "calling frenzy fcn" << std::endl;
	return _frenzy;
}

int game::Undead::resilience() const {
	return frenzy() ? 
		get_attribute("resilience_frenzy") : get_attribute("resilience");
}
bool game::Undead::action_AI(std::string const&)
{
	//std::cerr << "INSIDE ACTION_AI\n";
	if(_at_environment->characters().size()<=1) {
		return true;
	}
	else {
//		std::cerr << "Inside else statement \n" << (_at_environment->characters().size()-1) <<std::endl;;
		srand(time(NULL));
		int numb = rand()%(_at_environment->characters().size());
		auto it = _at_environment->characters().begin();
//		std::cerr << "random value is: " << numb << std::endl;

//		std::cerr << this->_at_environment->about_environment(this) << std::endl;
		
		for(int i = 0; i<numb ;++i) ++it;
		if (it->second == this) {
			if(numb+1>=_at_environment->characters().size()) {
				--it;
			}
			else {
				++it;
			}
		}
		return attack(it->second); /*attack(Character*) in Characer class*/
	}
}



void game::Undead::graveyard_effect() {
	std::cout << "Coming back to life...." << std::endl;
	_life += std::min(max_hp(), _life + get_attribute("hp_graveyard"));
}


bool game::Undead::action(std::string const& input = "AI") {
//	std::cout << "inside action for UNDEAD" << std::endl;
	update_mode();
//	if (frenzy()) std::cout << name() << " is completely rabid!" << std::endl;
	std::vector<std::string> subs = split_string(input);
	std::string key = subs[0];
	FPTR pf;
	try {
			pf = func_map.at(key);
	}catch(std::exception const& e)	{
		std::cout << "Invalid input - I don't understand what you want me to do!" << std::endl;
		return false;		
	}
	return (this->*pf)(input);
}


void game::Undead::create_func_map() {
	func_map["AI"] = &game::Undead::action_AI;
	func_map["go"] = &game::Undead::go;
	func_map["fight"] = &game::Undead::fight;


}

bool game::Undead::go(std::string const& input) {
	std::cerr << "inside GO functon" << std::endl;
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

bool game::Undead::fight(std::string const& input) {
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

std::string game::Undead::saveInfo(int x, int y) const {
	std::string info = type() + ":" +name() + ":" + to_string(health()) + ":" + to_string(x) + ":" + to_string(y);
	return info;
}

std::string game::Undead::to_string(int value ) const {
	std::ostringstream os;
	os << value;
	return os.str();	

}


/*
bool game::Undead::action(std::string const& input = "AI") {
//	std::cerr << "Inside action for undead" << std::endl;
	update_mode();
//	if (frenzy()) std::cout << name() << " is completely rabid!" << std::endl;
	std::vector<std::string> subs = split_string(input);
	if(subs.size() == 1 && subs[0] == "AI") {
		return action_AI(); 
	}
	else if (subs[0] == "go") {
		try {
			return walk(subs[1]);
		} catch (std::invalid_argument e) {
			std::cerr << "invalid argument to walk function caught" << std::endl;
			return false;
		}
		return false;
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
	std::cerr << "Invalid input - I do not understand!" << std::endl;
	return false; 
}*/