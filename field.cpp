//field.cpp 

#include "field.h"
/*
game::Field::Field() : Environment("Unkown field name") {}
*/
game::Field::Field(std::string str) : Environment(str) {}

std::string game::Field::type() const {
	return "Field";
}

std::string game::Field::add_information() const {
	return "It is a plain with clear sight. ";
}

