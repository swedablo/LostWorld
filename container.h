//container.h 
#ifndef container__h
#define container__h


namespace game{
	class Container;
}

#include "item.h"
#include "character.h"
#include "environment.h"
#include <vector>



class game::Container : public game::Item {

public:

	int capacity() const;
	int left_space() const;
	bool insert_item(Item*);
	bool remove_item(Item*);
	Character* is_open() const; // return nullptr, if not open.


	virtual std::vector< std::string> choice_item() const override;
	virtual bool use_item(Character*);

	// correctly assign pointer to current owner. 
	/*void set_owner(Character*);
	void set_owner(Environment*);*/

	virtual ~Container(); // tar bort alla element
	
	//Container(int, std::string, std::map<std::string,int>, std::string);
	//Container(int, std::string, std::string, int);
	Container(int, std::string, std::map<std::string, int>, std::string, std::vector<std::string>, int, Environment*, std::vector<Item*>);

	//Extra assignment
	std::string saveInfo(int, int) const override; 

//	Container(int, std::string, int);
//	Container(int, std::string, int, std::vector<Item*>);


protected:

	int const _capacity;
	Environment* _owner;
	std::vector<Item*> _containing;
	Character* _is_open;


	//Container();
	

};




#endif
