//graveyard.cpp 

#include "graveyard.h"
/*
game::Graveyard::Graveyard() : game::Environment() {

}*/

game::Graveyard::Graveyard(std::string str) : game::Environment(str) {

}


bool game::Graveyard::enter(Character* ch) {
	//std::cerr << "INSIDE ENTER IN GRAVEYARD, with " << ch->type() <<std::endl;
	if(ch->type() == "Priest" || ch->type()=="Undead") {
		if(_characters.count(ch->name()) > 0) {
			std::cerr << "Environment, function: ENTER. " << ch->name() << "was already in the environment " << this->about_environment(ch) << std::endl;
			return false;
		}
		else {
			std::cerr << ch->name() << " has entered the graveyard!" << std::endl;
			_characters[ch->name()] = ch;
			ch->graveyard_effect();
			return true;
		}
	}
	else {
		std::cout << ch->name() << " may not enter these domains!" << std::endl;
		return false;
	}
}

std::string game::Graveyard::add_information() const {
	return "This is a Graveyard full of tombstones. Calm and peaceful, yet a bit frightening!";
}

std::string game::Graveyard::type() const {
	return "Graveyard";
}
