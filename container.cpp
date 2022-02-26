//container.cpp 

#include "container.h"


game::Container::Container(int weight, std::string des, std::map<std::string, int> attr, std::string name, 
							std::vector<std::string> types, int cap, Environment* e, std::vector<game::Item*> cont = {})
 : game::Item(weight, des, attr, name, types), _capacity(cap), _owner(e), _containing(cont), _is_open(nullptr)
{
	e->add_item(this);
	for (auto it = _containing.begin(); it != _containing.end(); ++it)
	{
		if (_weight + (*it)->weight() <= capacity())
			_weight += (*it)->weight();
		else 
			std::cerr << "Error when constructing container: overloading." << std::endl;
	}
}

game::Container::~Container() {
}

int game::Container::capacity() const {
	return _capacity;
}

int game::Container::left_space() const {
	return (_capacity - _weight);
}

std::vector<std::string> game::Container::choice_item() const {
	std::vector<std::string> val;
	val.push_back("open");
	return val;
	//val.push_back("add item");
}	

bool game::Container::use_item(game::Character* ch) {
	if(ch->has_item("Magical Key")) {
		_is_open = ch;
		std::cout << "You open the chest and find the following..." << std::endl;
		for(auto it = _containing.begin(); it != _containing.end(); ++it) {
			std::cout << (*it)->name() << std::endl;
		}
		return true;
	}
	std::cout << "Shit, it has a lock. I need a key!" << std::endl;
	return false;
	/*
	std::vector<std::string> subs = this->split_string(str);
	if(subs[0] == "open"){
		if(subs[1] == "magic") {
			std::cerr << "The chest is now opened!! WIN WIN WIN " << std::endl;
			return true;
		}

	}*/
}

game::Character* game::Container::is_open() const {
	return _is_open;
}



bool game::Container::insert_item(Item* item) {
	std::cerr << "Inside INTER_ITEM, inserting item, " << item->get_description() << std::endl;
	if (_weight + item->weight() > _capacity)
	{
		std::cerr << "The item with description: " << item->get_description() << " ... was too heavy " << std::endl;
		return false;
	}
	else {
		_weight += item->weight();
		_containing.push_back(item);
		return true;
	}


}


bool game::Container::remove_item(Item* item) {
	std::cerr << "Inside REMOVE_ITEM, removing item, " << item->get_description() << std::endl;
	for(auto it = _containing.begin(); it != _containing.end(); ++it) {
		if (*it == item)
		{
			this->_weight -= (**it).weight();
			it = _containing.erase(it);
			return true;
		}
	}
	std::cerr << "the item, " << item->get_description() << " ... was not in the container" <<std::endl;
	return false;
}


std::string game::Container::saveInfo(int x, int y) const {
	std::string itemstr;
	for(auto it = _containing.begin(); it != _containing.end() ; ++it) {
		itemstr += "Chest_item:" + (*it)->name() + ":" + to_string((*it)->weight()) + ":" + get_description() + ":" + getAllowedClasses() + ":" + getSaveAttr() + ":" + to_string(x) + ":" + to_string(y) + "\n";
	}
	itemstr += "Item:" + this->name() + ":" + /*to_string(this->weight())*/"300" + ":" + this->to_string(this->capacity()) + ":" + this->get_description() + ":" + this->getAllowedClasses() + ":" + to_string(x) + ":" + to_string(y);
	return itemstr;
}






