//direction.cpp 
#include <algorithm>
#include <locale>
#include <stdexcept>
#include "direction.h"

game::Direction::Direction(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str == "north") _val = North;
	else if (str == "northeast") _val = Northeast;
	else if (str == "east") _val = East;
	else if (str == "southeast") _val = Southeast;
	else if (str == "south") _val = South;
	else if (str == "southwest") _val = Southwest;
	else if (str == "west") _val = West;
	else if (str == "northwest") _val = Northwest;
	else throw std::invalid_argument("Invalid direction specified.");
}

game::Direction::Direction(game::Direction::enum_type val)
	: _val(val)
{
	assert(val <= Northwest);
}

game::Direction::operator game::Direction::enum_type() const			//conversion to enum_type 
{
	return this->_val;
}

std::ostream& operator<< (std::ostream& os , game::Direction const& dir) {
	if(dir == 1) {
		os << "north";
	}
	else if (dir == 2)
	{
		os << "northeast";
	}
	else if (dir == 3)		
	{
		os << "east";
	}
	else if (dir == 4) {
		os << "southeast";
	}
	else if (dir == 5)
	{
		os << "south";
	}
	else if (dir == 6)	
	{
		os << "southwest";
	}
	else if (dir == 7)
	{
		os << "west";
	}
	else if (dir == 8)
	{
		os << "northwest";
	}
	return os;
}