#pragma once

#include "Card.h"

#include <eastl\fixed_map.h>

namespace Sonnet
{

typedef eastl::fixed_map<Card, unsigned int, TotalDifferentCards, false> CardCounts;

class CardDeck
{
public:
	CardDeck(Card myCard, Card myOtherCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& visibleDiscards);

	const Cards& getCards() const { return m_shuffledCards; }

	static Cards getPossibleCards(Card myCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& faceupDiscards);
	static float getWinProbality(Card myCard, const Cards& theirPossibleCards);

	static const CardCounts& getFullDeck();
	static const Cards s_allCards;

	static void initializeCardDeck();
private:
	static CardCounts s_fullDeck;
	

	static void initializeFullDeck();

	void initializeCardsFromVisible(Card myCard, Card myOtherCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& visibleDiscards);

	Cards m_shuffledCards;
};

}