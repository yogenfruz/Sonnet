#include "CardDecks.h"

#include <EASTL\algorithm.h>

namespace Sonnet
{

CardDecks::CardDecks(Card myCard, Card myOtherCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& visibleDiscards)
	: m_sortedCardDeck(myCard, myOtherCard, myPlayedCards, theirPlayedCards, visibleDiscards)
	, m_currentCardDeck(m_sortedCardDeck.getCards())
	, m_remainingShuffles(true)
{
	size_t totalDecks = factorial(m_currentCardDeck.size());

	size_t totalCombos = 1;

	for (Card card : CardDeck::s_allCards)
	{
		size_t numberCards = eastl::count(m_currentCardDeck.begin(), m_currentCardDeck.end(), card);
		if (numberCards == 0)
			numberCards = 1;

		totalCombos *= factorial(numberCards);
	}
	
	m_totalDecks = totalDecks / totalCombos;
}

bool CardDecks::getNextCardDeck(Cards& outCardDeck)
{
	outCardDeck = m_currentCardDeck;
	bool retVal = m_remainingShuffles;
	m_remainingShuffles = eastl::next_permutation(m_currentCardDeck.begin(), m_currentCardDeck.end());
	return retVal;
}

}