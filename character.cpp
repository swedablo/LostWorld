//character.cpp
#include "character.h"	
#include <stdexcept>	//exception caught for incorrect walk call
#include <sstream>		//split strings
#include <cmath>		//math for mod damage.
#include <stdlib.h> 	//RNG
#include <algorithm> 	//min function.
//#include <string>

game::Character::Character(std::string str, game::Environment* e ) : _life(0), _name(str), _at_environment(e)
{
	initiate_attributes();
	_status["health"] = &_life;
	if (_at_environment)
		_at_environment->add_character(this);
}

game::Character::~Character() {
	//std::cerr << "Character::destructor" <<std::endl;
	_at_environment->remove(this); 	// remove env. ptr to this
	//std::cerr << "Successful removal" << std::endl;
};	// virtual destructor. 

std::vector<std::string> game::Character::split_string(std::string const& input) const {
	std::stringstream ss(input);
	std::string substr;
	std::vector<std::string> subs;
	while (std::getline(ss, substr, ' ')) {
        subs.push_back(substr);
    }
	return subs;
}

void game::Character::initiate_attributes() {
	_attributes["max_hp"] = int(0);
	_attributes["min_dmg"] = int(0);
	_attributes["max_dmg"] = int(0);		
	_attributes["resilience"] = int(0);		// physical damage reduction.
}

int game::Character::generate_damage() const {
	return min_dmg() + rand() % (max_dmg() + 1 - min_dmg());
}

void game::Character::modify_damage(int& dmg) const {
	dmg = round(dmg*exp(-static_cast<double>(resilience())/100));
}
// PUBLIC

std::string game::Character::name() const {
	return _name;
}

bool game::Character::has_item(std::string str) const {
	return false;
}

std::vector<game::Item*> game::Character::carryingItems() const {
	std::vector<Item*> h;
	return h;
}


/*
bool game::Character::action(std::string const& input) { 
	// if loop implementation.
	// possibly purely virtual to account for different allowed actions. 
	std::vector<std::string> subs = split_string(input);
	if (subs[0] == "go") {
		try {
			walk(subs[1])
		} catch (std::invalid_argument e) {
			std::cerr << "invalid argument to walk function caught" << std::endl;
			return false;
		}
	}
	else if (subs[0] == "fight") {

	}
	else if () {


	}
	else if () {


	}

	}

}
*/
int game::Character::health() const {
	return this->_life;
}

bool game::Character::attacks_by_magic() const {
	return false;
}

std::vector <std::string > game::Character::get_attributes() const {
	std::vector <std::string> atts;
	for (auto it = _attributes.begin(); it != _attributes.end(); ++it) {
		atts.push_back(it->first);
	}	
	return atts;
}

int game::Character::get_attribute(std::string s) const {
	return _attributes.find(s)->second;
}

std::vector <std::string > game::Character::get_status() const {
	std::vector <std::string> status;
	for (auto it = _status.begin(); it != _status.end(); ++it) {
		status.push_back(it->first);
	}	
	return status;
}

int game::Character::get_stat(std::string s) const {
	return *((int const*)_status.find(s)->second);
}

int game::Character::max_hp() const {
	return _attributes.find("max_hp")->second;
}

int game::Character::min_dmg() const {
	return _attributes.find("min_dmg")->second;
}

int game::Character::max_dmg() const {
	return _attributes.find("max_dmg")->second;
}
	
int game::Character::resilience() const {
	return _attributes.find("resilience")->second;
}

std::vector <std::string> game::Character::allowed_actions() const {
	return {"go <direction>", "fight <target>", "display <attribute>"};
}

bool game::Character::attack(Character* target) {
	if(this == target) {
		std::cout << "Cannot attack yourself! " + this->name() << std::endl;
		return false;
	}
	std::cerr << this->name() << " is attacking " << target->name() << std::endl;
	target->taking_damage(generate_damage(), this);
	return true; 
}


bool game::Character::taking_damage(int dmg, Character* src) {
	/*std::cerr << "You, " << this->name() << " are being attacked by " << src->name() << ", " 
		<< src->type() << " for " << dmg << " damage." << std::endl;*/
	if (!src->attacks_by_magic()) {
		auto lambda_modify_damage = [&] (int& x) {
	//	std::cout << "INSIDE LAMBDA FUNCTION" << std::endl;
		x = round(dmg*exp(-static_cast<double>(resilience())/100)); }; // [&] since we are caputuring a reference variables, dmg.
		lambda_modify_damage(dmg);
	}
	std::cout << name() << " is taking " << dmg << " damage at " << health()
		      << " initial health." << std::endl;
	_life -= std::min(_life, dmg);
	return _life == 0;
}

bool game::Character::walk(game::Direction dir) {
	Environment* e_new = _at_environment->get_neighbour(dir);
	if (e_new) {
		if (e_new->enter(this)) {
			_at_environment->leave(this);
			_at_environment = e_new;
			return true;
		}
		else return false;
	}
	std::cerr << "Invalid environment - did not work. " << std::endl;
	return false; 
}


//Används för testing:
bool game::Character::walk(game::Environment* env) {
	std::cerr << "Inside walk in Character" << std::endl;
	if(_at_environment != nullptr) {
		if(_at_environment->leave(this)) {
			std::cerr << "Could leave the previos environment with description " << env->about_environment(this) << std::endl;
			if(env->enter(this)) {
				std::cerr << "Succeded in entering!! :D " << std::endl;
				_at_environment = env;
				return true;
			}
		}
	}
	else {
		std::cerr << "The character was not in a previous environment" << std::endl;
		if(env->enter(this)) {
			std::cerr << "Succeded in entering!! :D " << std::endl;
			_at_environment = env;
			return true;
		}
	}
	std::cerr << "COULD NOT WALK TO THIS Environment" << std::endl;
	return false;
}

game::Environment* game::Character::at_environment() const {
	return _at_environment;
}


void game::Character::graveyard_effect() {
}
