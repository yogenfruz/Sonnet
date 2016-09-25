#include "Card.h"
#include "CardDeck.h"

namespace Sonnet
{

constexpr unsigned int factorial(unsigned int n)
{
	return (n == 1) ? 1 : n * factorial(n - 1);
}

const unsigned int RemainingCards = TotalCards - 4;
const unsigned long long MaxShuffles = factorial(RemainingCards);

class CardDecks
{
public:
	CardDecks(Card myCard, Card myOtherCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, const Cards& visibleDiscards);

	bool getNextCardDeck(Cards& outCardDeck);

	unsigned int totalDecks() const
	{
		return m_totalDecks;
	}

private:
	CardDeck m_sortedCardDeck;
	Cards m_currentCardDeck;
	unsigned int m_totalDecks;
	bool m_remainingShuffles;
};

}