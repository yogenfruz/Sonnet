#include "CardDecks.h"

#include <EASTL\algorithm.h>

namespace Sonnet
{

CardDecks::CardDecks(Card myCard, Card myOtherCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& visibleDiscards)
	: m_sortedCardDeck(myCard, myOtherCard, myPlayedCards, theirPlayedCards, visibleDiscards)
	, m_currentCardDeck(m_sortedCardDeck.getCards())
	, m_remainingShuffles(true)
{
	m_totalDecks = factorial(m_currentCardDeck.size());
}

bool CardDecks::getNextCardDeck(Cards& outCardDeck)
{
	outCardDeck = m_currentCardDeck;
	bool retVal = m_remainingShuffles;
	m_remainingShuffles = eastl::next_permutation(m_currentCardDeck.begin(), m_currentCardDeck.end());
	return retVal;
}

}