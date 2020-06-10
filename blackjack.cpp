#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

// Xiaohui Wang 260719359
// Comp322 Assignment 2

// ACE cases are handled by both Hand.getTotal and HumanPlayer.announce

using namespace std;

// global scope for enum
enum Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
	EIGHT, NINE, TEN, JACK, QUEEN, KING };
// bind a char to enum type
enum Type {CLUBS = (int)'C', DIAMONDS = (int)'D', HEARTS = (int)'H', SPADES = (int)'S' };

// array to allow enumeration
static const Rank AllRank[] = { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
		EIGHT, NINE, TEN, JACK, QUEEN, KING };
static const Type AllType[] = {CLUBS, DIAMONDS, HEARTS, SPADES };

class Card
{
public:
	Rank rank; // variable of type enum
	Type type;

	// card constructor
	Card(Rank rankIn, Type typeIn){
		rank = rankIn;
		type = typeIn;
	}

	// retun numerical 1~10
	int getValue() const{

		if(this->rank==JACK || this->rank==QUEEN || rank==KING){
			return 10;
		}else{
			return (int)rank;
		}
	}

	void displayCard(){

		if(this->rank==JACK){
			cout << "J";
			cout << (char)type;
		}else if(this->rank==QUEEN){
			cout << "Q";
			cout << (char)type;
		}else if(this->rank==KING){
			cout << "K";
			cout << (char)type;
		}else{
			cout << to_string(getValue()) + "" + (char)type;
		}
	}

	~Card() {}
};

class Hand
{	
public:
	vector<Card> hand;

	// add card to the hand
	void add(Card card){
        hand.push_back(card);
	}

	void clear(){
		hand.clear();
	}

	// helper function for handling ACE special case
	bool hasAce() const{
		bool ace = false;
		for (int i=0; i<hand.size(); i++){
			if(hand[i].rank == ACE){
				ace = true;
			}
		}
		return ace;
	}

	// iterator to get sum of card values
	int getTotal() const{
		int res;

		int score = 0;
		for(vector<Card>::const_iterator it = hand.begin() ; it != hand.end(); ++it){
			score += it->getValue();
		}
		// In case of ACE, display preferred option (1 or 11)
		int orig = score;

		if(hasAce()){
			if((orig + 10) > 21){
				res = orig;
			}else{
				res = orig + 10;
			}
		}else{
			res = orig;
		}

		const int& rr = res;
		const_cast<int&>(rr) = res;

		return res;
	}

	~Hand() { }

};

// no inherit from class Hand
class Deck: public Hand
{
public:
	vector<Card> deck;

	// constructor to initialize deck
	Deck(){
		populate();
		shuffle();
	}

	//create deck of 52 cards (13*4)
	void populate(){
		// iterate enum
		// for (int rankInt = ACE; rankInt != Last; rankInt++){
		for (const auto r: AllRank){
			// r = static_cast<Rank>(rankInt);

			for (const auto t: AllType){

				// generate card
				deck.push_back(Card(r,t));
			}
		}
	}

	// shuffle cards
	void shuffle(){
		for(int i=52;i>1;i--)
		  {
			  int ind = rand() % i;
			  Card temp = deck[i-1];
			  deck[i-1] = deck[ind];
			  deck[ind] = temp;
		   }
	}

	// deal one card to a hand
	void deal(Hand &hand){
		Card card = deck.front(); // choose card
		hand.add(card); // add card to a hand

		deck.erase(deck.begin()); //remove element from deck

	}

	~Deck() {  }
};


// generic abstract player, inherit from Hand
class AbstractPlayer: public Hand
{
public:
	// tell whether player want to draw another card
	virtual bool isDrawing() const = 0;

	// check whether player has busted
	bool isBusted(){
		if (getTotal() <= 21)   // hand is inherited from Hand
			return false;
		else
			return true;
	}
};

class HumanPlayer: public AbstractPlayer
{
public:
    // inherited method for player: read user input
	virtual bool isDrawing() const{
		cout << "Do you want to draw? (y/n): ";
		char answer;
		cin >> answer;
		return (answer == 'y' ? true : false);
    }

    // print game situation
    void announce(Hand opponent){
    	string mes = "";

    	// calculate the final total with preferred (1 or 11) for both players
		int totalH;
		int totalComp;

		int origHuman = this->getTotal();

		if(this->hasAce() == true){
			if((origHuman + 10) > 21){
				totalH = origHuman;
			}else{
				totalH = origHuman + 10;
			}
		}else{
			totalH = origHuman;
		}

		int origComp = opponent.getTotal();

		if(opponent.hasAce() == true){
			if((origComp + 10) > 21){
				totalComp  = origComp;
			}else{
				totalComp  = origComp + 10;
			}
		}else{
			totalComp  = origComp;
		}

		// evaluate scores
    	if(isBusted()){
    		mes = "Player busts.\nCasino wins.";  // player busts
    	}else{
    		if(totalH > totalComp){
    			mes =  "Player wins.";
    		}
    		if(totalH == totalComp){
				mes = "Push: No one wins.";
			}
    		if(totalH < totalComp){
				mes = "Player Loses.";
			}
    	}

    	cout << mes << endl;
    }

    ~HumanPlayer() {  }

};

class ComputerPlayer: public AbstractPlayer
{
public:
	// inherited method for computer: keep drawing as sum<=16
	virtual bool isDrawing() const{
		if (getTotal() <= 16)
			return true;
		else
			return false;
	}
	~ComputerPlayer() {  }

};


class BlackJackGame
{
public:
	// have deck
	Deck m_deck;

	// two players
	HumanPlayer human;
	ComputerPlayer m_casino;

	// helper function to print hand situation
	void helper(Hand &player){
		for (int i=0; i<player.hand.size(); i++){
			player.hand[i].displayCard();
			cout << " ";
		}
	}

	void play(){

		// step 1: dealing cards to start
		m_deck.deal(m_casino);
		m_deck.deal(human);
		m_deck.deal(human);

		// display card info
		cout << "Casino: ";
		helper(m_casino);
		string casino_tot = " [" + to_string(m_casino.getTotal()) + "]";
		cout << casino_tot << endl ;

		// display 2 cards to user
		cout << "Player: ";
		helper(human);
		string human_tot = " [" + to_string(human.getTotal()) + "]";
		cout << human_tot << endl ;

		bool endRound = false;

		// step 2: ask for player decision - draw, no draw or busted
		bool drawChoice = human.isDrawing();
		while(drawChoice && !endRound){
			m_deck.deal(human);

			cout << "Player: ";
			helper(human);
			string human_tot = " [" + to_string(human.getTotal()) + "]";
			cout << human_tot << endl ;

			// case when player busts
			if(human.isBusted()){
				human.announce(m_casino);
				drawChoice = false;
				endRound = true;
			}else{
				drawChoice = human.isDrawing();  // loop or exit loop
			}

		}

		// step 3: computer draws card
		bool drawComp = m_casino.isDrawing();
		while(drawComp && !endRound){
			m_deck.deal(m_casino);

			cout << "Casino: ";
			helper(m_casino);
			int casino_total = m_casino.getTotal();
			string casino_tot = " [" + to_string(casino_total) + "]";
			cout << casino_tot << endl;

			// case when computer busts
			if(m_casino.isBusted()){    // interesting case when computer has ACE
				human.announce(m_casino);
				drawComp = false;
				endRound = true;
			}else{
				drawComp = m_casino.isDrawing();
			}

		}

		// step 4: compare results when neither busts, and restore game
		if(!endRound){
			human.announce(m_casino);
		}

		// clear data
		human.clear();
		m_casino.clear();
		m_deck.clear();

	}

};



/*
// for testing - all good
int main ()
{
	// testing Card - all good
	Card card1(QUEEN,DIAMONDS);
	const int val1 = card1.getValue();
	cout << val1 << endl;
	card1.displayCard();  // QD
	Card card2(KING,SPADES);
	const int val2 = card2.getValue();
	cout << val2 << endl;
	card2.displayCard(); // KS
	Card card3(THREE,CLUBS);
	const int val3 = card3.getValue();
	cout << val3 << endl;
	card3.displayCard();  // 3C

	// testing Hand - all good
	Hand hand1;
	hand1.add(card1);
	hand1.add(card2);
	hand1.add(card3);
	int sc1 = hand1.getTotal();
	cout << "total in handOne is " + to_string(sc1) << endl;
	hand1.clear();  // clear
	int sc2 = hand1.getTotal();
	cout << "after clear(), total in handOne is " + to_string(sc2) << endl;

	// testing Deck - all good
	Deck deck1;
	cout << "Size of deck is (should be 52): " + to_string(deck1.deck.size()) << endl;  // populated
	for (int i=0; i<5; i++){
		Card cc = deck1.deck[i];
		cc.displayCard(); // shuffled
	}
	int sc3 = hand1.getTotal();
	cout << "after clear(), total in handOne is " + to_string(sc3) << endl;
	deck1.deal(hand1); // dealt
	deck1.deal(hand1);
	int sc4 = hand1.getTotal();
	cout << "HandOne now has: " + to_string(hand1.hand.size()) << endl;
	cout << "Size of deck is (should be 50): " + to_string(deck1.deck.size()) << endl;

	// testing players - all good
	HumanPlayer human;
	ComputerPlayer computer;
	deck1.deal(human);
	cout << "Human player now has: " + to_string(human.hand.size()) << endl;
	cout << "Size of deck is (should be 49): " + to_string(deck1.deck.size()) << endl;
	human.announce(computer);
	human.isDrawing();

	cout << "Test over! \n" << endl ;
	return 0 ;
}
*/



