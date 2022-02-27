//priest.h  
#ifndef priest__h
#define priest__h

#include "human.h"
#include <string>
#include <vector>


namespace game {
	class Priest;
	// attribute constants.
	int const PRIEST_MAX_HP = 100;
	int const PRIEST_MIN_DMG = 15;
	int const PRIEST_MAX_DMG = 25;
	int const PRIEST_RESILIENCE = 0;
//	int const PRIEST_MAX_STAMINA = 100;
	int const PRIEST_CAPACITY = 50;
	int const PRIEST_MAX_ENLIGHTENMENT = 100;
	int const PRIEST_HEAL_POWER = 75;
	int const PRIEST_HEAL_COST = 35;
	int const PRIEST_ATTACK_COST = 25;
	int const PRIEST_MEDITATE_POWER = 65;
	int const PRIEST_GRAVEYARD_ENLIGHTENMENT = 25;
}

class game::Priest : public game::Human {

	protected:
		//in every phase, enlightment is increased
		int _enlightenment; // 
		
		void initiate_attributes() override; // hide that from base class - used at construction.

		typedef bool (Priest::*FPTR)(std::string const&); //
		std::map<std::string, FPTR> func_map;
		void create_func_map();
		std::string to_string(int) const;

	public:
		// construction and destruction. 
		/*
		Priest();
		explicit Priest(std::string name);*/
		Priest(std::string, Environment* e = nullptr);
		Priest(std::string, int, int, std::vector<std::string> const& str, Environment* e = nullptr); // Used for loading saved file

		bool attacks_by_magic() const override; // default false. priest: overload to true.
		// character status.
		int enlightenment() const;

		//attribute info
		int max_enlightenment() const;
		int heal_power() const;
		int heal_cost() const;
		int attack_cost() const;
		int meditate_power() const;
		// action info. 
		std::vector <std::string> allowed_actions() const override;
		// actions.
		bool action(std::string const&) override;
		bool attack(Character*) override; //damage
		//void action() override;
		//std::string action() override;
		std::string type() const override;
		
		//bool heal();	//heal himself
		void graveyard_effect() override;
		//int hit(); //Damage, Hit with hands, require no enlightment.

	
		bool go(std::string const& input);
		bool fight(std::string const& input);
		bool meditate(std::string const& input); //generate enlightenment & stamina;
		bool heal(std::string const& input); // heal himself
		bool pick(std::string const& input);
		bool use(std::string const& input);
		bool droping(std::string const& input);
		bool display(std::string const& input);
		bool info(std::string const& input);
		bool describe(std::string const& input);
		std::string saveInfo(int, int) const override;

};

#endif
