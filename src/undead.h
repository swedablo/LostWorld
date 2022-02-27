//undead.h 
#ifndef undead__h
#define undead__h

#include "character.h"
#include <string>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

namespace game {
	class Undead;
	int const UNDEAD_MAX_HP = 70;
	int const UNDEAD_MIN_DMG = 10;
	int const UNDEAD_MAX_DMG = 35;
	int const UNDEAD_RESILIENCE = 15;
	int const UNDEAD_RESILIENCE_FRENZY = -15;
	int const UNDEAD_HP_FRENZY = 30;
	int const UNDEAD_HP_GRAVEYARD = 20;
}

class game::Undead : public game::Character {

private:
	bool _frenzy;
	void update_mode(); //enter or exit frenzy mode. 
	bool action_AI(std::string const&);

protected: 
	void initiate_attributes() override;
	int generate_damage() const override;

	//Extra Assignment
	typedef bool (Undead::*FPTR)(std::string const&); //
	std::map<std::string, FPTR> func_map;
	void create_func_map();
	std::string to_string(int) const;

public:
	/*Undead(); // this one should maybe be protected. Cant instasiate unique name
	explicit Undead(std::string);*/
	Undead(std::string, Environment* e = nullptr); 
	Undead(std::string, int, Environment* e = nullptr); //used when loading saved file

	bool frenzy() const;	// is in or not in frenzy mode. 
	int resilience() const override; // lower when in frenzy mode. 
	std::string type() const override;
	bool action(std::string const&) override;
	void graveyard_effect() override;


	//Extra assigment
	bool go(std::string const& input);
	bool fight(std::string const& input);
	std::string saveInfo(int, int) const override;
};


#endif
