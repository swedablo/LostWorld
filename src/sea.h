//field.cpp 

#ifndef sea__h
#define sea__h

#include "environment.h"

namespace game {
	class Sea;
}

class game::Sea : public Environment {
protected:
	std::string add_information() const override;

public:
	//Sea();
	explicit Sea(std::string);
	bool enter(Character*) override;
	virtual std::string type() const;
	virtual std::string about_environment(Character const*) const override;



};

#endif

