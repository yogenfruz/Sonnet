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

Card cardNumberToCard(unsigned int cardNumber, bool allowZero)
{
	switch (cardNumber)
	{
	case 0:
		if (allowZero)
			return NoCard;
		else
			return InvalidCard;
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


Card getCard(const char* reason, bool zeroForDone = false)
{
	using std::cin;
	using std::cout;
	using std::endl;

	Card card = InvalidCard;
	writeCards();
	cout << reason << endl;
	unsigned int cardNumber = ~0u;
	while (card == InvalidCard)
	{
		cout << "Enter a card number: " << endl;
		while (!(cin >> cardNumber))
		{
			cin.clear();
			cin.ignore(eastl::numeric_limits<std::streamsize>::max(), '\n');
		}
		card = cardNumberToCard(cardNumber, zeroForDone);
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

Card getMyCard()
{
	return getCard("Please enter my card.");
}

Card getMyOtherCard()
{
	return getCard("Please enter my other card.");
}

Cards getMyPlayedCards()
{
	Cards myPlayedCards;

	while (Card myPlayedCard = getCard("Enter the next card I've already played/discarded.", true))
	{
		if (myPlayedCard != NoCard)
			myPlayedCards.push_back(myPlayedCard);
		else
			break;
	}
	
	return myPlayedCards;
}

Cards getTheirPlayedCards()
{
	Cards theirPlayedCards;

	while (Card theirPlayedCard = getCard("Enter the next card they've already played/discarded.", true))
	{
		if (theirPlayedCard != NoCard)
			theirPlayedCards.push_back(theirPlayedCard);
		else
			break;
	}

	return theirPlayedCards;
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

	vgs.faceupDiscards = getFaceupDiscards();
	vgs.myCard = getMyCard();
	vgs.myOtherCard = getMyOtherCard();

	vgs.myPlayedCards = getMyPlayedCards();
	vgs.theirPlayedCards = getTheirPlayedCards();

	auto result = minimaxStart(vgs);

	cout << "I want to play " << cardToString(result.cardToPlay) << " with an expected win percentage of " << result.expectedWinPercentage << "%" << endl;

	cin.get();

	return 0;

}