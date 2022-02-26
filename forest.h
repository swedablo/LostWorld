//forest.h 
#ifndef forest__h
#define forest__h



namespace game {
	class Forest;
}

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>   

#include "environment.h"



class game::Forest : public game::Environment {

	protected:
		std::string add_information() const override;
	public:
		//Forest();
		explicit Forest(std::string const);
		//std::string about_environment() const override;
		std::string type() const override;
		std::string about_environment(Character const*) const override;
		virtual bool leave(Character*) override;

		//game::Direction* get_directions() const override;

};




#endif
