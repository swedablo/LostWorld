#ifndef warrior__h
#define warrior__h

#include "human.h"
#include <vector>
#include <string>

namespace game {
	class Warrior;

	int const WARRIOR_MAX_HP = 150;
	int const WARRIOR_MIN_DMG = 25;
	int const WARRIOR_MAX_DMG = 35;
	int const WARRIOR_RESILIENCE = 20;
	int const WARRIOR_CAPACITY = 120;
	int const WARRIOR_MAX_STAMINA = 100;
	int const WARRIOR_STAMINA_FIGHT = 25;
	int const WARRIOR_STAMINA_WALK = 10;
	int const WARRIOR_STAMINA_REST = 70;
}


class game::Warrior : public game::Human {
protected:
	int _stamina;
	void initiate_attributes() override;

	//Extra Assignment
	typedef bool (Warrior::*FPTR)(std::string const&); //
	std::map<std::string, FPTR> func_map;
	void create_func_map();
	std::string to_string(int) const;

public:
	/*Warrior();
	explicit Warrior(std::string name);*/
	Warrior(std::string, Environment* e = nullptr);
	Warrior(std::string, int, int,std::vector<std::string> const&, Environment* e = nullptr); // used to load saved file

	// character status.
	int stamina() const;

	//attribute info
	int max_stamina() const;
	int stamina_fight() const;
	int stamina_walk() const;
	int stamina_rest() const;

	// action info. 
	std::vector <std::string> allowed_actions() const override;
	// actions.
	bool action(std::string const&) override;
	bool attack(Character*) override; //damage
	//void rest();	// regain stamina. 
	bool walk(Direction) override;	// costs stamina for warrior.
	//void action() override;
	//std::string action() override;
	std::string type() const override;


	bool go(std::string const& input);
	bool fight(std::string const& input);
	bool pick(std::string const& input);
	bool use(std::string const& input);
	bool droping(std::string const& input);
	bool display(std::string const& input);
	bool info(std::string const& input);
	bool describe(std::string const& input);
	bool rest(std::string const& input);
	std::string saveInfo(int, int) const override;


};

#endif // warrior__h
