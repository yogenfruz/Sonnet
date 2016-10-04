#pragma once

#include "Card.h"

#include <iostream>
#include <iomanip>
#include <EASTL/numeric_limits.h>

namespace Sonnet
{

	const char* cardToString(const Card card)
	{
		switch (card)
		{
		case Card::Guard:
			return "Grd";
		case Card::Priest:
			return "Pst";
		case Card::Baron:
			return "Brn";
		case Card::Handmaid:
			return "Hnd";
		case Card::Prince:
			return "Prn";
		case Card::King:
			return "Kng";
		case Card::Countess:
			return "Cts";
		case Card::Princess:
			return "Pnc";
		}
		return "what?";
	}

typedef signed int GameScore;

const GameScore KeepPlaying = 0, Draw = 0;
const GameScore MyWin = 10000;
const GameScore TheirWin = -10000;
const signed int PointsRange = MyWin - TheirWin;

struct TurnResult
{
	signed int gameScore;
	bool gameOver;
};

enum class TurnEvaluate
{
	KeepPlaying,
	CheckGuard,
	CompareCardsBaron,
	InstantLose,
};

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

TurnEvaluate isGameOver(const GameState& gameState)
{
	const Card& playedCard = gameState.playedCard;

	if (gameState.guardState.guessActive)
		return TurnEvaluate::CheckGuard;

	if (gameState.triggerBaron)
		return TurnEvaluate::CompareCardsBaron;

	if (playedCard & Princess)
		return TurnEvaluate::InstantLose;

	return TurnEvaluate::KeepPlaying;
}

TurnResult isWin(const GameState& gameState, bool maximizingPlayer)
{
	TurnEvaluate turnEvaluation = isGameOver(gameState);
	switch (turnEvaluation)
	{
	case TurnEvaluate::KeepPlaying:
	{
		return TurnResult{ KeepPlaying, false };
	}
	case TurnEvaluate::CheckGuard:
	{
		Card guessCard = gameState.guardState.guessCard;
		Card targetCard = (!maximizingPlayer ? gameState.myCard : gameState.theirCard);
		GameScore whoWon = (maximizingPlayer ? MyWin : TheirWin);
		if (guessCard == targetCard)
		{
			return TurnResult{ whoWon, true };
		}
		return TurnResult{ KeepPlaying, false };
	}
	case TurnEvaluate::InstantLose:
	{
		return TurnResult{ TheirWin, true };
	}
	case TurnEvaluate::CompareCardsBaron:
	{
		const Card& myCard = gameState.myCard;
		const Card& theirCard = gameState.theirCard;
		if (myCard > theirCard)
		{
			return TurnResult{ MyWin, true };
		}
		else if (myCard < theirCard)
		{
			return TurnResult{ TheirWin, true };
		}
		else
		{
			return TurnResult{ KeepPlaying, false };
		}
	}
	}

	// Special case to force us to play Countess when we have King/Prince
	const Card& targetPlayerCard = (maximizingPlayer ? gameState.myCard : gameState.theirCard);
	const bool targetCardIsKingOrPrince = (targetPlayerCard == King ? true : false) || (targetPlayerCard == Prince ? true : false);
	if (gameState.playedCard == Countess && targetCardIsKingOrPrince)
	{
		return TurnResult{ (maximizingPlayer ? TheirWin : MyWin) , true };
	}

	return TurnResult{ KeepPlaying, false };
}

struct MiniMaxResult
{
	GameScore score;
};

typedef eastl::fixed_vector<GameState, 64, false> GameStates;

void doPlayCard(Card playedCard, const GameState& oldGameState, bool maximizingPlayer, GameStates& outGameStates)
{
	GameState newState = oldGameState;
	if (maximizingPlayer)
		newState.myState = PlayerState();
	else
		newState.theirState = PlayerState();
	newState.princeState = PrinceState();
	newState.guardState = GuardState();
	const PlayerState& otherPlayer = maximizingPlayer ? newState.theirState : newState.myState;
	if (otherPlayer.handmaidProtected)
	{
		if (playedCard == Prince)
		{
			newState.princeState.discardMe = true;
			outGameStates.push_back(newState);
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
			possibleCards.push_back(newState.burnCard);
			eastl::copy(newState.faceupDiscards.begin(), newState.faceupDiscards.end(), possibleCards.begin());
			Card otherCard = (maximizingPlayer ? newState.theirCard : newState.myCard);
			possibleCards.push_back(otherCard);
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
		else if (playedCard == Princess)
		{
			outGameStates.push_back(newState);
		}
	}
	
}

MiniMaxResult minimax(const GameState& gameState, bool maximizingPlayer)
{
	TurnResult result = isWin(gameState, !maximizingPlayer);
	if (result.gameOver == false && gameState.remainingDeck.size() == 0)
	{
		result.gameOver = true;
		GameScore winningScore;
		if (gameState.myCard > gameState.theirCard)
			winningScore = MyWin;
		else if (gameState.myCard < gameState.theirCard)
			winningScore = TheirWin;
		else
			winningScore = Draw;
		result.gameScore = winningScore;
	}
	if (result.gameOver)
	{
		return MiniMaxResult {result.gameScore};
	}

	unsigned int numberDecks = 0;
	GameState newState = gameState;
	Card drawCard = newState.remainingDeck.pop_back();
	GameStates gameStates;
	if (maximizingPlayer)
	{
		{
			// Play old card
			GameState playedOldCard = newState;
			Card playedCard = playedOldCard.myCard;
			playedOldCard.myCard = drawCard;
			doPlayCard(playedCard, playedOldCard, true, gameStates);
		}
		{
			// Play new card
			GameState playedNewCard = newState;
			Card playedCard = drawCard;
			doPlayCard(playedCard, playedNewCard, true, gameStates);
		}
		
		GameScore bestScore = TheirWin;
		const GameState* bestGameState = nullptr;

		for (const GameState& childState : gameStates)
		{
			GameScore thisScore = minimax(childState, false).score;
			if (thisScore >= bestScore)
			{
				bestGameState = &childState;
				bestScore = thisScore;
			}
		}
		
		return MiniMaxResult{ bestScore };
	}
	else
	{
		{
			// Play old card
			GameState playedOldCard = newState;
			Card playedCard = playedOldCard.myCard;
			playedOldCard.theirCard = drawCard;
			doPlayCard(playedCard, playedOldCard, false, gameStates);
		}
		{
			// Play new card
			GameState playedNewCard = newState;
			Card playedCard = drawCard;
			doPlayCard(playedCard, playedNewCard, false, gameStates);
		}

		GameScore bestScore = MyWin;
		const GameState* bestGameState = nullptr;

		for (const GameState& childState : gameStates)
		{
			GameScore thisScore = minimax(childState, true).score;
			if (thisScore <= bestScore)
			{
				bestGameState = &childState;
				bestScore = thisScore;
			}
		}

		return MiniMaxResult{ bestScore };

	}
}

struct VisibleGameState
{
	Card myCard;
	Card myOtherCard;
	Cards myPlayedCards;
	Cards theirPlayedCards;

	Cards faceupDiscards;
};

struct MoveToPlay
{
	GameScore expectedScore;
	Card cardToPlay;
};

MoveToPlay minimaxStart(VisibleGameState state)
{
	CardDecks decks(state.myCard, state.myOtherCard, state.myPlayedCards, state.theirPlayedCards, state.faceupDiscards);

	const unsigned int totalDecks = decks.totalDecks();

	struct Play
	{
		Card card = InvalidCard;
		Card guessCard = InvalidCard; // For guard
		bool discardMe = false; // For prince
		bool operator< (const Play& rhs) const
		{
			if (card < rhs.card)
				return true;
			if (card > rhs.card)
				return false;

			if (guessCard < rhs.guessCard)
				return true;
			if (guessCard > rhs.guessCard)
				return false;

			if (discardMe && !rhs.discardMe)
				return true;

			return false;
		}

	};

	struct PlayResult
	{
		unsigned int timesPlayed;
		signed long long totalScore;
	};

	typedef eastl::fixed_map<Play, PlayResult, 16> CardsPlayed;

	CardsPlayed cardTotals;

	unsigned int decksPlayed = 0;

	Cards deck;

	while (decks.getNextCardDeck(deck))
	{	
		for (Card playedCard : {state.myCard, state.myOtherCard})
		{
			Cards deckCopy = deck;
			Card theirCard = deckCopy.pop_back();
			Card burnCard = deckCopy.pop_back();

			GameState potentialGameState(state.myCard, state.myPlayedCards, state.theirPlayedCards, theirCard, state.faceupDiscards, deckCopy, burnCard);
			potentialGameState.playedCard = playedCard;
			potentialGameState.myCard = (playedCard == state.myCard ? state.myOtherCard : state.myCard);
			if (state.theirPlayedCards.size() > 0 && state.theirPlayedCards.back() == Handmaid)
				potentialGameState.theirState.handmaidProtected = true;
			GameStates outcomeGameStates;
			doPlayCard(playedCard, potentialGameState, true, outcomeGameStates);
			for (GameState outcomeGameState : outcomeGameStates)
			{
				MiniMaxResult searchResult = minimax(outcomeGameState, false);

				GameScore computedScore = searchResult.score;

				Play playedPlay;
				playedPlay.card = playedCard;
				if (playedCard == Guard)
				{
					playedPlay.guessCard = outcomeGameState.guardState.guessCard;
				}
				if (playedCard == Prince)
				{
					playedPlay.discardMe = outcomeGameState.princeState.discardMe;
				}

				if (cardTotals.find(playedPlay) == cardTotals.end())
				{
					PlayResult newResult = { 1, computedScore };
					cardTotals[playedPlay] = newResult;
				}
				else
				{
					PlayResult& result = cardTotals[playedPlay];
					result.timesPlayed++;
					result.totalScore += computedScore;
				}


			}
		}
		decksPlayed++;
		if (decksPlayed % 1000 == 0)
			std::cout << "Evaluated " << decksPlayed << " of " << totalDecks << std::endl;
	}

	std::cout << "Done evaluation moves" << std::endl;
		
	Play bestPlay;
	double bestScore = eastl::numeric_limits<double>::lowest();

	for (const auto& pair : cardTotals)
	{
		const Play& playedPlay = pair.first;
		unsigned int timesPlayed = pair.second.timesPlayed;
		signed long long totalScore = pair.second.totalScore;

		std::cout << "Played a " << cardToString(playedPlay.card) << " - " << timesPlayed << " times." << std::endl;
		if (playedPlay.card == Guard)
			std::cout << "Guessed " << cardToString(playedPlay.guessCard) << std::endl;
		if (playedPlay.card == Prince)
			std::cout << "Discarded my cards with Prince? " << (playedPlay.discardMe ? " yes" : " no") << std::endl;

		double computedScore = totalScore / (double)timesPlayed;
		double percentChance = computedScore + (MyWin);
		percentChance = percentChance / (double)PointsRange * 100.f;

		std::cout << "Percent chance: " << std::fixed << std::setprecision(2) << percentChance << "%" << std::endl;

		if (computedScore > bestScore)
		{
			bestScore = computedScore;
			bestPlay = playedPlay;
		}
	}

	return MoveToPlay{ (GameScore)bestScore, bestPlay.card };
}



}