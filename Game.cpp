#include "Game.h"

namespace Sonnet
{

void doPlayCard(Card playedCard, const GameState& oldGameState, bool maximizingPlayer, GameStates& outGameStates)
{
	GameState newState = oldGameState;
	if (maximizingPlayer)
		newState.myState = PlayerState();
	else
		newState.theirState = PlayerState();
	newState.princeState = PrinceState();
	newState.guardState = GuardState();

	newState.playedCard = playedCard;

	KnowledgeState& knowledgeLoser = (maximizingPlayer ? newState.theirKnowledgeState : newState.myKnowledgeState);
	if (knowledgeLoser.knownCard == playedCard)
		knowledgeLoser = KnowledgeState();

	const PlayerState& otherPlayer = maximizingPlayer ? newState.theirState : newState.myState;
	if (otherPlayer.handmaidProtected)
	{
		if (playedCard == Prince)
		{
			newState.princeState.discardMe = true;
			if (newState.myCard != Princess)
			{
				outGameStates.push_back(newState);
			}			
			newState.myPlayedCards.push_back(playedCard);
			if (newState.remainingDeck.size() > 0)
				newState.myCard = newState.remainingDeck.pop_back();
			else
			{
				newState.myCard = newState.burnCard;
				newState.burnCard = InvalidCard;
			}
			newState.princeState.discardMe = false;
			outGameStates.push_back(newState);
		}
		else
		{
			outGameStates.push_back(newState);
		}
	}
	else
	{
		if (playedCard == Guard)
		{
			Cards possibleCards;
			eastl::copy(newState.remainingDeck.begin(), newState.remainingDeck.end(), eastl::back_inserter(possibleCards));
			possibleCards.push_back(newState.burnCard);
			Card otherCard = (maximizingPlayer ? newState.theirCard : newState.myCard);
			possibleCards.push_back(otherCard);
			const KnowledgeState& knownCard = (maximizingPlayer ? newState.myKnowledgeState : newState.theirKnowledgeState);
			if (knownCard.knownCard != InvalidCard && knownCard.knownCard != Guard)
			{
				possibleCards.clear();
				possibleCards.push_back(knownCard.knownCard);
			}
			for (const Card& guessCard : possibleCards)
			{
				if (guessCard == Guard)
					continue;

				GuardState guardState;
				guardState.guessActive = true;
				guardState.guessCard = guessCard;
				newState.guardState = guardState;
				outGameStates.push_back(newState);
			}
		}
		else if (playedCard == Priest)
		{
			KnowledgeState& knowledgeGainer = (maximizingPlayer ? newState.myKnowledgeState : newState.theirKnowledgeState);
			Card knownCard = (maximizingPlayer ? newState.theirCard : newState.myCard);
			knowledgeGainer.knownCard = knownCard;
			outGameStates.push_back(newState);
		}
		else if (playedCard == Baron)
		{
			newState.triggerBaron = true;
			outGameStates.push_back(newState);
		}
		else if (playedCard == Handmaid)
		{
			PlayerState& activePlayer = maximizingPlayer ? newState.myState : newState.theirState;
			activePlayer.handmaidProtected = true;
			outGameStates.push_back(newState);
		}
		else if (playedCard == Prince)
		{
			for (bool discardMe : { true, false})
			{
				Cards& discards = (discardMe ? newState.myPlayedCards : newState.theirPlayedCards);
				Card& activeHand = (discardMe ? newState.myCard : newState.theirCard);
				if (discardMe == true && activeHand == Princess)
					continue;
				discards.push_back(activeHand);
				if (newState.remainingDeck.size() > 0)
					activeHand = newState.remainingDeck.pop_back();
				else
				{
					activeHand = newState.burnCard;
					newState.burnCard = InvalidCard;
				}

				newState.princeState.discardMe = discardMe;

				outGameStates.push_back(newState);
			}

		}
		else if (playedCard == King)
		{
			eastl::swap(newState.myCard, newState.theirCard);
			outGameStates.push_back(newState);
		}
		else if (playedCard == Countess)
		{
			outGameStates.push_back(newState);
		}
		else if (playedCard == Princess)
		{
			outGameStates.push_back(newState);
		}
	}
}

}

