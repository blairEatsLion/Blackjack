/*
 * blackjack.h
 * Author: Xiaohui Wang
 * containing all declarations of classes
 */

// global scope (enum)
enum Rank;
enum Type;
static const Rank AllRank[];
static const Type AllType[];

// class declarations
class Card {
public:
	Rank rank;
	Type type;
	Card(Rank rankIn, Type typeIn);
	int getValue() const;
	void displayCard();
	~Card();
};

class Hand {
public:
	vector<Card> hand;
	void add(Card card);
	void clear();
	bool hasAce();   // helper function
	int getTotal() const;
	~Hand();
};

class Deck: public Hand {
public:
	vector<Card> deck;
	Deck();
	void populate();
	void shuffle();
	void deal(Hand &hand);
	~Deck();
};

class AbstractPlayer: public Hand {
public:
	virtual bool isDrawing() const = 0;
	bool isBusted();
};

class HumanPlayer: public AbstractPlayer {
public:
	virtual bool isDrawing() const;
	void announce(Hand opponent);
	~HumanPlayer();
};

class ComputerPlayer: public AbstractPlayer {
public:
	virtual bool isDrawing() const;
	~ComputerPlayer();
};

class BlackJackGame {
public:
	Deck m_deck;
	HumanPlayer human;
	ComputerPlayer m_casino;
	void helper(Hand &player);
	void play();
};




