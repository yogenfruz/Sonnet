#include "CardDeck.h"

#include <random>

namespace Sonnet
{

const Cards CardDeck::s_allCards = { Card::Guard, Card::Priest, Card::Baron, Card::Handmaid,
	Card::Prince, Card::King, Card::Countess, Card::Princess };

CardCounts CardDeck::s_fullDeck;

CardDeck::CardDeck(Card myCard, Card myOtherCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& visibleDiscards)
{
	initializeCardsFromVisible(myCard, myOtherCard, myPlayedCards, theirPlayedCards, visibleDiscards);
}


Cards CardDeck::getPossibleCards(Card myCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& faceupDiscards)
{
	CardCounts cards = getFullDeck();
	Cards returnCards;

	cards[myCard]--;

	for (const Card& visibleCard : myPlayedCards)
	{
		cards[visibleCard]--;
	}

	for (const Card& visibleCard : theirPlayedCards)
	{
		cards[visibleCard]--;
	}

	for (const Card& visibleCard : faceupDiscards)
	{
		cards[visibleCard]--;
	}

	for (const auto& cardPair : cards)
	{
		for (unsigned int i = 0; i < cardPair.second; ++i)
		{
			returnCards.push_back(cardPair.first);
		}
	}

	return returnCards;
}

float CardDeck::getWinProbality(Card myCard, const Cards & theirPossibleCards)
{
	const unsigned int totalPossibilities = theirPossibleCards.size();
	unsigned int totalWins = 0;

	for (const Card& possibleCard : theirPossibleCards)
	{
		if (myCard > possibleCard)
			totalWins++;
	}

	return (float)totalWins / totalPossibilities;
}

void CardDeck::initializeCardDeck()
{
	CardDeck::initializeFullDeck();
}

void CardDeck::initializeFullDeck()
{
	const int bitmaskFiveCards = Card::Guard;
	const int bitmaskTwoCards = Card::Priest | Card::Baron | Card::Handmaid | Card::Prince;
	const int bitmaskOneCard = Card::King | Card::Countess | Card::Princess;
	for (Card card : s_allCards)
	{
		if (card & bitmaskFiveCards)
			s_fullDeck[card] = 5;
		else if (card & bitmaskTwoCards)
			s_fullDeck[card] = 2;
		else if (card & bitmaskOneCard)
			s_fullDeck[card] = 1;
	}
}

const CardCounts& CardDeck::getFullDeck()
{
	return s_fullDeck;
}

void CardDeck::initializeCardsFromVisible(Card myCard, Card myOtherCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& visibleDiscards)
{
	CardCounts cards = getFullDeck();

	cards[myCard]--;
	cards[myOtherCard]--;

	for (const Card& visibleCard : myPlayedCards)
	{
		cards[visibleCard]--;
	}

	for (const Card& visibleCard : theirPlayedCards)
	{
		cards[visibleCard]--;
	}

	for (const Card& visibleCard : visibleDiscards)
	{
		cards[visibleCard]--;
	}

	for (const auto& cardPair : cards)
	{
		for (unsigned int i = 0; i < cardPair.second; ++i)
		{
			m_shuffledCards.push_back(cardPair.first);
		}
	}
}

}