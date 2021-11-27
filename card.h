#ifndef _CARD_H_
#define _CARD_H_

#include <ostream>

enum COLOR { wild, red, green, blue, yellow}; 

class card
{
	public:
		int number; // 0-9 numbers, +2, skip, reverse, all color, +4 (all color) 
		COLOR color; // 5 colors: red, green, blue, yellow, and no color 

		// check for a card is equal to a another card
		bool operator==(card const & other) const;

		// check for a card is not equal to a another card
		bool operator!=(card const & other) const;
		
		card(); 
		card(int num, COLOR col); 
}; 

// display a card by "<<" operator
std::ostream & operator<<(std::ostream & out, card const & temp_card);


#endif // _CARD_H_
