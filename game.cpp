
#include "card.h"
#include "deck.h"
#include "player.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <cstdlib> // std::system() ; std::rand()
#include <time.h>
#include <random>
using namespace std;

int amount_players;
int playerWon;
int score[10] = { 0 };
int gameRound=1;
char playType;

int main() {

	string dataToGUI = "";	// data which send to gui
	int flag = 0;
	string playerCards;		// cards of other players

	//checking for marks of players
	for (int i = 0; i < (amount_players); i++) {	
		if (score[i] != 0) {
			flag = 1;
			break;
		}
	}

	//checking the round number
	if (gameRound > 1) {	
		system("cls");
		cout << "\n----*****  STARTING GAME ROUND " << gameRound << "  ****----\n\n";
		Sleep(4000);
	}
	else
		cout << "\n----*****  STARTING GAME ROUND " << gameRound << "  ****----\n\n";

	while (flag == 0)
		{
			cout << "Select the game mode....\n\n";
			cout << "\t1 - Play With Computer\n";
			cout << "\t2 - Multi Player\n\n";
			cout << "Select your chooice : ";
			cin >> playType;

			cout << "Please enter amount of players: ";
			cin >> amount_players;
			if (amount_players >= 2 && amount_players <= 10)
			{
				cout << amount_players << " Players are entering to the game .... " << endl;
				flag = 1;
				break;
			}
			else
			{
				cout << "Invalid amount of players !!!" << endl;
			}
		}

	// create the components of the game
	deck main_deck;			// creating deck 
	main_deck.create();
	main_deck.quick_shuffle();
	player* play_array;		// creating player array 
	play_array = new player[amount_players];

	// distributing 7 starting cards to each player 
	for (int i = 0; i < amount_players; i++)
	{
		for (int k = 0; k < 7; k++)
		{
			card temp_card;
			temp_card = main_deck.draw();		//draw a card from the deak
			play_array[i].hand_add(temp_card);	//give the drawn card to the player
		}
	}

	//all cards that are played will go to temp_deck
	deck temp_deck;
	//creating the first starting card
	card played_card;
	card temp_card;
	int card_flag = 0;

	//fist card should not be a wild card.. if it is re draw a card from the deak
	while (card_flag == 0)
	{
		temp_card = main_deck.draw();	// initially there is always card in the middle

		if (temp_card.color != wild)
		{
			card_flag = 1;
			played_card = temp_card;
		}
		//if first card is wild, redraw
		else
		{
			temp_deck.add_card(temp_card);
		}
	}

	// generate random number
	srand(time(NULL));
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> rndm(0, amount_players-1);

	int turn;
	if (gameRound == 1) {
		turn = rndm(gen) + 10 * amount_players;		// to select the first player to draw
		cout << "\nPLAYER " << turn - 10 * amount_players << " is randomly selected to play first" << endl;
		Sleep(2000);
	}
	else {
		turn = playerWon+ 10 * amount_players;
		cout<< "\nPLAYER " << turn - 10 * amount_players << " is the dealer now !!!" << endl;
		Sleep(2000);
		turn++;
	}

	bool force_draw_bool = false;	// if a wild, card, skip, Reverse ,Draw-Two card selected
	int turn_flag = 1;				// define the turn leftword or rightword
	int win = 0;					// if one player wins
	
	while (win == 0)	//keep playing until a player wins the round
	{
		Sleep(2000);
		system("cls");

		player* curr_player = &play_array[turn % amount_players];

		// checked for forced draw cards
		if (force_draw_bool)	// if a Draw-4 or Draw-2 played by last player
		{
			// Draw-2
			if (played_card.number == 10)
			{
				cout << "Forced Draw-2" << endl;
				card draw_2;
				for (int i = 0; i < 2; i++)			// draw 2 cards
				{
					draw_2 = main_deck.draw();		// draw from main deck 
					curr_player->hand_add(draw_2);	// add the card to the players hand
				}
			}

			// Draw-4
			if (played_card.number == 14)
			{
				cout << "Forced Draw-4" << endl;
				card draw_4;
				for (int i = 0; i < 4; i++)			// draw 4 cards
				{
					draw_4 = main_deck.draw();		// draw from main deck 
					curr_player->hand_add(draw_4);	// add the card to the players hand
				}

			}
			force_draw_bool = false;	// this will affect only to a one player
		}

		if (turn % amount_players == 0|| playType=='2') {

			cout << "\nPlayed Card: " << played_card << endl << endl;
			if(playType == '2')
			system("pause");

			cout << "\nCards remaining for each player: " << endl;
			for (int i = 0; i < amount_players; i++)
			{
				cout << "PLAYER " << i << ": " << play_array[i].get_size() << "   "; //array of all players

				if (turn % amount_players == 0) {
					dataToGUI += to_string(play_array[i].get_size());	//add current players no of cards
					dataToGUI += "|";
				}
			}
		}

		// print out cards in player's hand
		cout << "\nPLAYER " << turn % amount_players << endl;

		if (turn % amount_players == 0 || playType == '2') {
			// we are player 0 and only our card lisst should be displayed
			dataToGUI += to_string(played_card.number);
			dataToGUI += to_string(played_card.color);
			dataToGUI += curr_player->print(0);			// get my card deck to the string variable
		}
		else
			playerCards = curr_player->print(1);	//get other players cards

		int check_flag = 0;
		int index;
		int size = curr_player->get_size();				// get current players no of cards

		// ask for which card to play into middle
		while (check_flag == 0)
		{
			// user input of the player 
			if (turn % amount_players == 0 || playType == '2') {
				cout << "If you want to draw a card please enter '-1' " << endl;
				cout << "Enter the index of the card..... ";

				// check for the uno input of the player when having onlya one card
				string unoTest;
				if (curr_player->get_size() == 1)
				{
					cin >> unoTest;
					if (unoTest == "-1")
						index = -1;
					else if (unoTest == "uno")
						index = 0;
					else
					{
						card draw_2;
						for (int i = 0; i < 2; i++)		// draw 2 cards
						{
							draw_2 = main_deck.draw();		// draw from main deck 
							curr_player->hand_add(draw_2);	// add the card to the players hand
							index = 99;
						}
					}
				}
				else {
					cin >> index;
					dataToGUI = "";
				}
			}
			// input by the computer
			else {
				char myHandC[100] = {};
				string temp[30][2] = {};
				int count = 0;
				strcpy_s(myHandC, playerCards.c_str());	//get the current players hand in to a char array

				for (int i = 0; i < 100; i++) {

					if (myHandC[i] == '|')	// devide the card color and the number by "|" mark
						count++;			// count the no of cards
					else if (count >= size)
						break;
					else {
						if (myHandC[i] == 'r' || myHandC[i] == 'g' || myHandC[i] == 'b' || myHandC[i] == 'y')	// get the color of a card 
							temp[count][0] += myHandC[i];
						else			//get the number of a card
							temp[count][1] += myHandC[i];
					}
				}
				string playedColor;
				switch (played_card.color)	// change the numeric color varibable in to string playedColor
				{
				case 0: playedColor = "w";  break;		// wild card
				case 1: playedColor = "r";  break;		// red card
				case 2: playedColor = "g";  break;		// green card
				case 3: playedColor = "b";  break;		// blue card
				case 4: playedColor = "y";  break;		// yellow card
				default: playedColor = ""; break;
				}

				int tempFlag = 0;	// to check whether the player have a playble card
				for (int x = 0; x < size; x++) {
					if (playedColor == temp[x][0]) {		// if the player have same color card
						tempFlag = 1;
						index = x;
					}

					if (to_string(played_card.number) == temp[x][1]) {	// if the player have same number card
						index = x;
						tempFlag = 1;
					}
				}

				if (tempFlag == 0) {
					for (int x = 0; x < size; x++) {
						if (temp[x][0] == "w") {		// if the player doesnt have maching color but if the player have a wild card
							index = x;
							tempFlag = 1;
						}
						else
							index = -1;
					}
				}
				
			}

			//check if index is to draw a card
			if (index == -1)
			{
				card draw_temp;
				draw_temp = main_deck.draw();
				cout << "DRAWN CARD : " << draw_temp << endl;
				if (draw_temp == played_card && draw_temp.color != wild)
				{
					int play_draw_flag = 0;
					while (play_draw_flag == 0)
					{
						string temp_play;

						if (turn % amount_players == 0 || playType == '2') {
							cout << "Do you want to play the drawn card [y/n] : ";
							cin >> temp_play;
						}
						else
							temp_play = "n";

						temp_play = "y";
						if (temp_play == "y")
						{
							played_card = draw_temp;
							temp_deck.add_card(draw_temp);
							if (played_card.number >= 10 && played_card.number <= 14)
							{
								force_draw_bool = true;
							}
							play_draw_flag = 1;
						}
						if (temp_play == "n")
						{
							curr_player->hand_add(draw_temp);
							play_draw_flag = 1;
						}
					}
				}
				else
				{
					curr_player->hand_add(draw_temp);
				}
				check_flag = 1;

			}

			if (index == 99)
				check_flag = 1;
			//validate the index
			else if (index >= 0 && index < size)
			{
				// check if card is compatilbe with played card
				card temp = curr_player->peek(index);
				if (temp == played_card)
				{
					//remove from player's hand
					curr_player->hand_remove(index);
					// add to the discarded pile
					temp_deck.add_card(temp);
					// change the played card
					played_card = temp;
					// check if card is a wild card
					if (played_card.color == wild)
					{
						COLOR temp_color;
						int colNum;

						if (turn % amount_players == 0 || playType == '2') {
							// ask for new color
							cout << "Please choose a color (red=1 , green=2, blue=3, yellow=4) :";
							cin >> colNum;
							while (colNum > 4 && colNum < 1)
							{
								cout << "Entered value is not a valid value !!!!!\n\n";
								cout << "Please choose a color (red=1 , green=2, blue=3, yellow=4) :";
								cin >> colNum;
							}
						}
						else
							colNum = rand() % 4 + 1;

						switch (colNum) {
						case 1: temp_color = red;  break;		// red card
						case 2: temp_color = green;  break;		// green card
						case 3: temp_color = blue;  break;		// blue card
						case 4: temp_color = yellow;  break;	// yellow card
						}
						played_card.color = temp_color;
					}
					if (played_card.number >= 10 && played_card.number <= 14)
					{
						force_draw_bool = true;
					}
					check_flag = 1;
				}
				else	// entered index is invalid
					cout << "Card cannot be played " << endl;
			}
			else {
				if (turn % amount_players == 0 || playType == '2') {
					if(index!=-1)
						cout << "Invalid index " << endl;
				}
			}
		}

		// check if there is a winner, and break while loop
		if (curr_player->get_size() == 0) {
			gameRound++;
			win = 1;
			cout << "PLAYER " << turn % amount_players << " has won the round." << endl;

			playerWon = turn % amount_players;

			for (int i = 0; i < (amount_players); i++) {

				//checking the hand remaining cards of players except who won
				string cardNum;
				player* playerSelect = &play_array[i];
				if (playerSelect != curr_player) {
					string round = playerSelect->print(1);
					int cards[4];
					
					int a = 1, value;
					for (int k = 0; k < play_array[i].get_size(); k++) {
						cardNum = "";

						while (round[a] != '|') {
							cardNum += round[a];
							a++;
						}
						//calculate the score
						value = stoi(cardNum);
						if (value < 10)
							score[playerWon] += value;
						else if (value < 13)
							score[playerWon] += 20;
						else
							score[playerWon] += 50;
						a += 2;
					}
				}
			}
			//showing the score of the round
			cout << "\n-------Scores-------\n";
			for (int i = 0; i < (amount_players); i++) {
				cout << "Player " << i << ": " << score[i] << endl;
			}
			Sleep(5000);	//to pause screen for 5sec

			// checking any player have 500 or more score
			for (int i = 0; i < (amount_players); i++) {
				if (score[i] >= 500) {
					cout << "\nPLAYER " << i << " has won the game.\n" << endl;
					return 0;
				}
			}
			main();		//to start the next round
		}
		
		if (played_card.number == 11 && force_draw_bool == true) {
			if (turn_flag == 1)
				turn = turn + 2;
			else
				turn = turn - 2;
		}
		// reverse case
		else if (played_card.number == 12 && force_draw_bool == true) {
			// if only two players, behaves like a skip card
			if (amount_players == 2)
				turn = turn + 2;
			else {
				// changes the rotation of game (from CW to CCW or vice versa)
				if (turn_flag == 1) {
					turn_flag = -1;
					turn--;
				}
				else {
					turn_flag = 1;
					turn++;
				}
			}
		}
		// for other cards
		else {
			// depending on going Clockwise or CounterClockwise
			if (turn_flag == 1)
				turn++;
			else
				turn--;
		}
		cout << "\nPlayed Card: " << played_card << endl << endl;	//showing the played card
		
	}
	return 0;
}