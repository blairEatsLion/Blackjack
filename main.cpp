#include <iostream>
#include <string>
#include <vector>
#include "blackjack.cpp"
//#include "blackjack.h"  // please comment this line when compile, otherwise "error: redefinition of ..."

// Xiaohui Wang 260719359
// Comp322 Assignment 2, Mar 11, 2019

using namespace std;

int main ()
{
	cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl ;

	BlackJackGame game;

	// The main loop of the game
	bool playAgain = true ;
	char answer = 'y' ;
	while (playAgain)
	{
		game.play();
		
		// Check whether the player would like to play another round
		cout << "Would you like another round? (y/n): " ;
		cin >> answer;
		cout << endl << endl ;
		playAgain = (answer == 'y' ? true : false );
	}

	cout << "Gave over!" ;
	return 0 ;
}

