//sea.cpp 

#include "sea.h"


//game::Sea::Sea() : Environment() {}

game::Sea::Sea(std::string str) : Environment(str) {} 

std::string game::Sea::type() const {
	return "Sea";
}

std::string game::Sea::add_information() const {
	return "Vast, blue oceans. You cannot go there. ";
}

std::string game::Sea::about_environment(Character const*) const {
	return (this->type() + ". ") + add_information();
}

bool game::Sea::enter(Character*) {
	std::cout << "Trying to walk into the sea eh? Bad idea!" << std::endl;
	return false;
}

