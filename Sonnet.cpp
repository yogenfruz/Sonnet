// Sonnet.cpp : Defines the entry point for the console application.
//

#include "Card.h"
#include "CardDeck.h"
#include "CardDecks.h"
#include "MinMax.h"

#include <iostream>

int main()
{
	using namespace Sonnet;
	CardDeck::initializeCardDeck();

	VisibleGameState vgs;
	vgs.myCard = Prince;
	vgs.myOtherCard = Priest;

	vgs.faceupDiscards = { Guard, Handmaid, Countess };
	vgs.myPlayedCards = { Guard, Guard, Prince };
	vgs.theirPlayedCards = { Guard, Guard, Priest, King, Handmaid };

	auto result = minimaxStart(vgs);

	std::cin.get();

	return 0;
}