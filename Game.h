#pragma once

#include "Card.h"

#include <EASTL/fixed_vector.h>

namespace Sonnet
{

struct PlayerState
{
	bool handmaidProtected = false;
};

struct GuardState
{
	bool guessActive = false;
	Card guessCard = InvalidCard;
};

struct PrinceState
{
	bool discardMe = false;
};

struct KnowledgeState
{
	Card knownCard = InvalidCard;
};

struct GameState
{
	Card myCard;
	Cards myPlayedCards;
	Card theirCard;
	Cards theirPlayedCards;
	Cards faceupDiscards;
	Cards remainingDeck;
	Card burnCard;

	Card playedCard;

	PlayerState myState;
	PlayerState theirState;

	GuardState guardState;

	PrinceState princeState;

	KnowledgeState myKnowledgeState;
	KnowledgeState theirKnowledgeState;

	bool triggerBaron = false;

	GameState(Card myCard, const Cards& myPlayedCards, const Cards& theirPlayedCards, Card theirCard, const Cards& faceupDiscards, const Cards& remainingDeck, Card burnCard)
		: myCard(myCard)
		, myPlayedCards(myPlayedCards)
		, theirCard(theirCard)
		, theirPlayedCards(theirPlayedCards)
		, faceupDiscards(faceupDiscards)
		, remainingDeck(remainingDeck)
		, burnCard(burnCard)
	{

	}

private:

};

typedef eastl::fixed_vector<GameState, 64, false> GameStates;

void doPlayCard(Card playedCard, const GameState& oldGameState, bool maximizingPlayer, GameStates& outGameStates);

}