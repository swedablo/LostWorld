//character.h
#ifndef character__h
#define character__h


namespace game {
	class Character;
	//int const life_default = 100; 
}


#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "environment.h"
#include "direction.h"
#include "item.h"
#include <sstream>

class game::Character {
protected:
	int _life;		
	std::string _name;
	Environment* _at_environment;	//current character environment. 
	std::unordered_map< std::string, int > _attributes; // character changeable int attributes, such as maximum health. 
	std::unordered_map< std::string, void* > _status; // give access to character life, mana etc. 
	
	explicit Character(std::string, Environment* e = nullptr);


	// help functions
	std::vector<std::string> split_string(std::string const&) const; // split string into substrings, to use for parsing.	
	virtual void initiate_attributes(); // add common character attributes to _attributes upon instantiation. derived classes may override this. 
	virtual int generate_damage() const; // function to generate damage. undead overloads this.
	void modify_damage(int&) const; // damage reduction formula given character resilience value. 


public:
	virtual ~Character();	// virtual destructor., 
	
	std::string name() const;		//name of the character
	virtual bool attacks_by_magic() const; // default false. priest: overload to true.
	virtual bool action(std::string const& = "AI") = 0;	// action function -> call character's map to decide what action to perform. 
										// false if input string is incorrect. -> ask player to perform new action.
										// all classes implement this depending on their available actions.

	virtual std::string type() const = 0; // Return the type of the character



	// character status.
	int health() const;
	std::vector < std::string > get_status() const; // return vector of all char status strings
	int get_stat(std::string) const; // get value of specific stat given by input string
	virtual bool has_item(std::string) const; // check if char. is holding an item with specific name. default false. 
	virtual std::vector<Item*> carryingItems() const; // Grund är tom std::vector. Human överlagrar.

	// attribute info.
	std::vector < std::string > get_attributes() const;
	int get_attribute(std::string) const; 
	int max_hp() const;		// maximum health. 
	int max_dmg() const;
	int min_dmg() const;
	virtual int resilience() const; //undead overrides.

	// action info.
	virtual std::vector <std::string> allowed_actions() const;
	Environment* at_environment() const;

	// standard character actions.
	virtual bool attack(Character*);		// base damage: random number between min and max damage. 
	virtual bool taking_damage(int, Character*);// take damage from attack. true if attack kills. 
	virtual bool walk(Direction); // change to environment object specified by direction, return false if not allowed. 
	virtual bool walk(Environment*); // används för att testa

	virtual void graveyard_effect();	// effect when entering graveyard.
										// default: nothing happens. 

	//Version update
	virtual std::string saveInfo(int,int) const = 0;

	
};



#endif
