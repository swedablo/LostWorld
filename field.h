//field.h 
#ifndef field__h
#define field__h

#include "environment.h"

namespace game {
	class Field;
}


class game::Field : public game::Environment {

	public:
		//Field();
		explicit Field(std::string);
		std::string type() const override;

	protected:
		std::string add_information() const override;
		
};


#endif

