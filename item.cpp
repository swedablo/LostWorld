//item.cpp 

#include "item.h"

/*
game::Item::Item() : _weight(10), _description("Unknown (from Item default constructor)") {
}

game::Item::Item(int a, std::string str) : _weight(a), _description(str) {
}
*/
game::Item::Item(int a, std::string str , std::map<std::string, int> map, std::string namn, std::vector<std::string> types = std::vector<std::string>())
 : _weight(a), _description(str), _static_attr(map), _name(namn), _classes(types) {}

game::Item::~Item() {
/*	if (_container)
		_container->remove(this);*/
}

std::string game::Item::get_description() const {
	return _description;
}


void game::Item::describe() const {
	std::cout << _name << ": " << _description << std::endl;
	std::cout << "This item has weight " << weight() << "." << std::endl;
}

int game::Item::weight() const {
	return _weight; 
}

std::string game::Item::name() const {
	return _name;
}

std::map<std::string, int> game::Item::get_static_attr() const {
	return this->_static_attr;
}

std::vector<std::string> game::Item::choice_item() const {
	return std::vector<std::string>();
}

bool game::Item::use_item(std::string str) {
	return false;
}

bool game::Item::use_item(game::Character* ) {
	return false;
}

std::vector < std::string > const& game::Item::types() const {
	return _classes;
}

std::vector<std::string> game::Item::split_string(std::string const& input) const {
    std::stringstream ss(input);
    std::string substr;
    std::vector<std::string> subs;
    while (std::getline(ss, substr, ' ')) {
        subs.push_back(substr);
    }
    return subs;
}

std::string game::Item::saveInfo(int x, int y) const {
//	std::cout << "ITEM: Inside saveInfo\n";
	std::string str = "Item:" + name() + ":" + to_string(weight()) + ":" + get_description() + ":" + getAllowedClasses() + ":" + getSaveAttr() +":" + to_string(x) + ":" + to_string(y);
	return str;
}

std::string game::Item::getSaveAttr() const {
//	std::cout << "ITEM: Inside getSaveAttr()\n";
	std::string str;
	for(auto it = _static_attr.begin() ; it != _static_attr.end(); ++it ) {
		str += it->first + "-" + to_string(it->second) + ":";
	}
//	std::cout << "ITEM: Finished\n";
	return str;

}

std::string game::Item::getAllowedClasses() const {
//	std::cout << "ITEM: Inside getAllowedClasses\n";
	std::string str;
	for (auto it = _classes.begin() ; it != _classes.end() ; ++it) {
		str += *it + ":";
	}
//	std::cout << "ITEM: Finished\n";
	return str;

}

std::string game::Item::to_string(int value ) const {
	std::ostringstream os;
	os << value;
	return os.str();	

}




/*
std::map<std::string, int> game::Item::use_item() {
	return _active_attr;
}
*/

