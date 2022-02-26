//object.h
#ifndef item__h
#define item__h

namespace game {
	class Item;
}

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>        //split strings
#include "character.h" // Since we need use_item(Character* ) in container



class game::Item {
	
		
	public:

		//item();
		//explicit item(std::string);
		//Item(int, std::string);
		explicit Item(int, std::string, std::map<std::string, int>, std::string, std::vector<std::string>);
		//Item();
		virtual ~Item();


		std::map<std::string, int> get_static_attr() const;

		virtual std::vector< std::string> choice_item() const; // Returnerar val som man kan göra med denna item. Öppna den (ifall kontainer), konsumera. Alternativt statisk vilket returnerar en tom vector 
		virtual bool use_item(std::string);
		virtual bool use_item(Character*);
		
		std::string name() const;	
		std::string get_description() const; // Gives a description of this item
		int weight() const; 
		std::vector < std::string > const& types() const;
		void describe() const;

		virtual std::string saveInfo(int, int) const;
	protected:
		int _weight; //For containers this one could change
		std::string const _description;
		std::map<std::string, int> _static_attr; // max_hp, min_dmg, max_dmg, resilience, capacity, max_enlightenment, heal_power, heal_cost, attack_cost, meditate_power
		std::string const _name;
		std::vector<std::string> const _classes;  // list of class types that benefit from item. 
		/*Item* _container;
		void set_container();
		void reset_container();*/
		//std::map<std::string, int> _active_attr;

		std::string to_string(int) const;

		std::vector<std::string> split_string(std::string const& input) const;

		std::string getSaveAttr() const;
		std::string getAllowedClasses() const;
	
};



#endif
