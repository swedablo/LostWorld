//human.h 
#ifndef human__h
#define human__h

#include "character.h"
#include "item.h"
#include <vector>
#include <string>


namespace game {
	class Human;
}

//This is the abstract-class that a player can be

class game::Human : public game::Character {

	protected:
//		int _stamina;	// move to warrior.
		std::vector<Item*> _carrying;

		void update_stats(); 					// behaves differently depending on derived class?
		std::string itemsCarried() const;
	public:
		/*Human();
		explicit Human(std::string);*/
		Human(std::string, Environment* e = nullptr);
		~Human() override;	// overload this to put items back in environment. 
		
		// attribute info
		int capacity() const;		// carrying capacity. - added to _attributes. 

		// character status
		std::vector<Item const*> carried_items() const; // const * : we can not change item properties here. 
		int carried_weight() const; // total weight of carried items. 
		//int stamina() const;		// 

		// action info 
		std::vector <std::string> allowed_actions() const override;
		// human specific methods - do not consume turns. 
		bool has_item(std::string item) const override;
		bool use_item(std::string const&);
		bool pick_up(std::string const&);	// identify item by string. 
		bool pick_up(Item*); // true if you can. false if it is too heavy
		bool drop(std::string const&); //identify item by string. 
		bool drop(Item*);	// drop object - does not consume turn.
		void display_items() const; // display carried items on screen.  

		std::string saveCarriedItems(int, int) const;


		

		
	//the characters that we can instantiate.
};








#endif
