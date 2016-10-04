// Sonnet.cpp : Defines the entry point for the console application.
//

#include "Card.h"
#include "CardDeck.h"
#include "CardDecks.h"
#include "MinMax.h"

#include <iostream>

namespace Sonnet
{

void writeCards()
{
	using std::cout;
	using std::endl;

	cout << "1 - Guard" << endl;
	cout << "2 - Priest" << endl;
	cout << "3 - Baron" << endl;
	cout << "4 - Handmaid" << endl;
	cout << "5 - Prince" << endl;
	cout << "6 - King" << endl;
	cout << "7 - Countess" << endl;
	cout << "8 - Princess" << endl;

}

Card cardNumberToCard(unsigned int cardNumber)
{
	switch (cardNumber)
	{
	case 1:
		return Guard;
	case 2:
		return Priest;
	case 3:
		return Baron;
	case 4:
		return Handmaid;
	case 5:
		return Prince;
	case 6:
		return King;
	case 7:
		return Countess;
	case 8:
		return Princess;
	default:
		return InvalidCard;
	}
	return InvalidCard;
}


Card getCard(const char* reason)
{
	using std::cin;
	using std::cout;
	using std::endl;

	Card card = InvalidCard;
	while (card == InvalidCard)
	{
		writeCards();
		cout << "Enter a card number: " << endl;
		cout << reason << endl;
		unsigned int cardNumber = ~0u;
		cin >> cardNumber;
		card = cardNumberToCard(cardNumber);
	}
	return card;
}

Cards getFaceupDiscards()
{
	Cards faceupDiscards;
	for (unsigned int i = 1; i < 4; i++)
	{
		Card faceupDiscard = getCard("Please enter the next faceup discard");
		faceupDiscards.push_back(faceupDiscard);
	}
	return faceupDiscards;
}

}

int main()
{
	using namespace Sonnet;
	CardDeck::initializeCardDeck();

	using std::cout;
	using std::cin;
	using std::endl;

	cout << "Welcome to Sonnet" << endl;

	VisibleGameState vgs;
	vgs.myCard = Guard;
	vgs.myOtherCard = Baron;

	vgs.faceupDiscards = getFaceupDiscards();
	vgs.myPlayedCards = { Prince };
	vgs.theirPlayedCards = { Countess, Priest };

	auto result = minimaxStart(vgs);

	cin.get();

	return 0;

}