//forest.cpp 

#include "forest.h"



std::string game::Forest::type() const {
	return "Forest";
}

//This will not be used in this class since the whole "about environment" is overrided
std::string game::Forest::add_information() const {
	return "You cannot see much. There are some weird noises...";
}

std::string game::Forest::about_environment(game::Character const* src) const {
	//std::cerr << "Inside about_environment" << std::endl;
	if(src!= nullptr && _characters.count(src->name())>0)
		return _name + ". This is a " + this->type() + ". " + this->add_information() + "\n" + this->show_characters() + "\n" + this->show_items();
	else {
		return _name + ". This is a " + this->type() + ". " + this->add_information() + ". ";
	}
}


/*
game::Forest::Forest() : game::Environment() {
//	this->_description= "This is a Forest. You cannot see much. There are some weird noises...";
}*/


game::Forest::Forest(std::string const str) : Environment(str) {
//	this->_description= "This is a Forest. You cannot see much. There are some weird noises... " + str;
}

bool game::Forest::Forest::leave(Character* pch) {
	srand(time(NULL));
	int numb = rand()%10+1;
	if(numb == 1) {
		std::cout << "Shit! I am lost. Cannot find my way out of here... Give me a moment..." << std::endl;
		return false;
	}
	else if(_characters.count(pch->name())==0) {
		std::cerr << "Environment, function: LEAVE. " << pch->name() << " didn't exist in the environment" << std::endl;	
		return false;
	}
	else {
		std::cerr << pch->name() << " Now leaves the environemtn " << std::endl;
		_characters.erase(pch->name());
		return true;
	}
	
}

/*
game::Direction* get_directions() const () {
	
}*/

/*
game::Forest::Forest(std::string descript = 'n_21_') : _description(descript){
	if (descript == 'n_21_') 
		_description = "This in a Forest. You cannot see much. There are some weird noises...";
]

*/
