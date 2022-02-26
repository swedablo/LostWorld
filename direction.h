#ifndef direction__h
#define direction__h

#include <cassert>
#include <iostream>

namespace game {

	class Direction;

}

// http://stackoverflow.com/questions/482745/namespaces-for-enum-types-best-practices source. 
class game::Direction
{
public:
    typedef enum
    {	
		None,
		North, 
		Northeast,
		East,
		Southeast,
		South,
		Southwest,
		West,
		Northwest
    } enum_type;

private:
    enum_type _val;

public:
	Direction(std::string str);		//convert string to direction. 
    Direction(enum_type val = None);

   // friend std::ostream& operator<<(std::ostream& os , Direction const&);
    operator enum_type() const;			//conversion to enum_type 
};

std::ostream& operator<< (std::ostream& os, game::Direction const&);

#endif // direction__h