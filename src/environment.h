//environment.h 
#ifndef environment__h
#define environment__h

namespace game {
	class Environment;

}

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <ostream>


#include "item.h"
#include "character.h"
#include "direction.h"


class game::Environment {

	protected:
		std::string const _name;
		std::vector<game::Item* > _items; // the objects that are in this environment
		std::map<std::string,Character*> _characters;
		std::map<Direction,Environment*> _neighbours;
		

		std::string show_characters() const;
		std::string show_items() const;
	
	//	explicit Environment();
		explicit Environment(std::string const);

		
		virtual std::string add_information() const = 0; // Used to add extra descriptions about the environment. 
		
	public:
		
		virtual ~Environment();
		virtual std::string about_environment(Character const*) const; // Character pointer to determine what information the character
		virtual std::string type() const = 0;
		virtual bool enter(/*std::shared_ptr<Character>*/ Character*); // - Some children can redefine this
		void add_character(Character*); //new character added to env. upon instantiation
		void remove(Character*); //remove character ptr upon destruction
		std::string name() const;
		
		Environment* get_neighbour(Direction) const;
		void add_neighbours(std::map<Direction,Environment*>);
		std::map<Direction, Environment*> show_neighbours() const;
				
		std::vector<Item* > const& items();		// return item list.

		virtual bool leave(Character*); // Då char lämnar alt dör
		bool remove_item(Item* );				// remove item from environment. 
		bool add_item(Item* );					// add new free item to environment. 
		
		std::map<std::string, Character*> const& characters() const;
		std::string to_string(int) const; 
	
		
};



#endif
