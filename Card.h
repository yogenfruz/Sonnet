#pragma once

#include <eastl\fixed_vector.h>

namespace Sonnet
{

enum Card
{
	Guard = 1 << 0,
	Priest = 1 << 1,
	Baron = 1 << 2,
	Handmaid = 1 << 3,
	Prince = 1 << 4,
	King = 1 << 5,
	Countess = 1 << 6,
	Princess = 1 << 7,
	InvalidCard = 1 << 8,
};

const unsigned int NumFaceupDiscardedCards = 3;
const unsigned int NumDiscardedCards = 4;
const unsigned int TotalDifferentCards = 8;
const unsigned int TotalCards = 16;

typedef eastl::fixed_vector<Card, TotalCards, false> CardsBase;

class Cards : public CardsBase
{
public:
	Card pop_back()
	{
		Card drawnCard = back();
		CardsBase::pop_back();
		return drawnCard;
	}

	Cards(std::initializer_list<Card> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_VECTOR_DEFAULT_ALLOCATOR)
		: CardsBase(ilist, overflowAllocator)
	{ }

	Cards()
		: CardsBase()
	{ }
	
};

}