//graveyard.h 

#ifndef graveyard__h
#define graveyard__h

namespace game {
	class Graveyard;
}

#include "environment.h"

class game::Graveyard : public game::Environment {
public:
	//Graveyard();
	explicit Graveyard(std::string);
	virtual bool enter(Character*) override;
	virtual std::string type() const override;

protected:
	virtual std::string add_information() const override;
};

#endif
