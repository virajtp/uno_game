
#define DECK_SIZE 108

#include "deck.h"
#include "card.h"
#include <iostream>		
#include <cstdlib> 
#include <time.h>
using namespace std;

deck::deck()
{
	ptr_deck = new card[DECK_SIZE];	// initialize deack of 108 cards
	size = 0;
}

// create the deck with colors and numbers
void deck::create()
{
	int num = 0 ;	// card no (red 0, yellow 0)


	// deck structure 
	/** 0		1	2	3	4	5	6	7	8	9		10			11			12			13				14
		0*4		   --numbers*8--						Draw Two*8	Reverse*8	skip*8		Wild Cards*4	Wild Draw Four*4
																							
	**/

	// create all colores(4) 0 cards
	for (int col = 1 ; col <= 4  ; col++) {
		ptr_deck[size].number = num;	//y initialize 0 card
		ptr_deck[size].color = static_cast<COLOR>(col);	// convert intrger in to color type 
		size++;
	}

	// create card no (1-9)*4*2 , (draw-two)4*2, (skip)4*2, reverse)4*2		---> 4 colors * 2 rows
	for (num = 1; num <= 12; num++)	{				// 12 cards each color
		for (int x = 0 ; x < 2 ; x++ ) {			// 2 rows
			for (int col = 1 ; col <= 4  ; col++) {	// 4 colors
				ptr_deck[size].number = num;
				ptr_deck[size].color = static_cast<COLOR>(col);		// convert intrger in to color type 
				size++;
			}
		}
	}

	// create wild-cards*4 and wild-drawfour-cards*4
	for (num = 13 ; num <= 14 ; num++) {
		for (int x = 0 ; x < 4 ; x++) {
			ptr_deck[size].number = num;
			ptr_deck[size].color = wild;
			size++;
		}
	}
}

deck::deck(const deck & other) {
	copy(other);
}


const deck & deck::operator= (const deck & other) {
	if (this != &other) {
		clear();
		copy(other);
	}
	return *this;
}

deck::~deck() {
	clear();
}

void deck::shuffle()
{
	// create a temp deck
	card * temp_deck = new card[size];
	for (int i =0; i<size; i++)
		temp_deck[i] = ptr_deck[i];
	
	int temp_size = size;
	int temp_pos;
	int pos;
	for (int i = 0 ; i <size; i++ ) {
		// randomly find a position in temp_deck
		srand(time(NULL)); // change the seed , so randomized number is different each time
		pos = rand() % temp_size;
		//assign random element to deck[i]
		ptr_deck[i] = temp_deck[pos];

		// removing element from temp_deck (O(n) to shift)
		temp_size--;
		for (temp_pos = pos ; temp_pos < temp_size ; temp_pos++)
			temp_deck[temp_pos] = temp_deck[temp_pos+1];
	}

	delete [] temp_deck;
}
card deck::draw() {
	
	if (size <= 0)//if the cards in the deck is over		
		return card();

	card temp_card = ptr_deck[size-1];	// draw the last card of the deak to the playear
	size--;								// change the last card index
	return temp_card;
}

// added card will go to a new dock which size start from 0
int deck::add_card(card temp_card) {
	// total drawn cards should no exceed the deak size
	if(size < DECK_SIZE)
	{
		ptr_deck[size] = temp_card;	// card will add to deak	
		size++;
		return 0;
	}
	else
		return -1;
}

// 
void deck::quick_shuffle() {

	/* 
	 rand() generates a random number start from last element
	swap it with randomly selected from whole array including last
	now array is from 0 to n-2(size reduced by 1 ) and repeat the process until we hit first element
*/
	int pos;
	int temp_size = size-1;			// last value of size is 108 but it should be 107
	card temp_card;
	while(temp_size > 0 )			// to shuffle all the cards in the deak
	{
		//generate random
		srand(time(NULL));			// change the seed , so randomized number is different each time
		pos = rand() % temp_size;	// generates a random numbe
		// swap current card with random selected card
		temp_card = ptr_deck[temp_size];
		ptr_deck[temp_size] = ptr_deck[pos];
		ptr_deck[pos] = temp_card;
		// reduce size by 1
		temp_size--;
	}

}



void deck::print_deck() {
	for (int i= 0 ; i< size ; i++)
		cout << i << ": " << ptr_deck[i] << endl;
}

void deck::copy(const deck & other) {
	size = other.size;
	ptr_deck = new card[size];
	for (int i =0 ; i < size ; i++)
		ptr_deck[i] = other.ptr_deck[i];
}


void deck::clear() {
	delete []ptr_deck;
	ptr_deck = NULL;
	size = 0;
}


int deck::get_size() {
	return size;
}
