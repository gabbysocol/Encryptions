#pragma once

#include "BigInteger\BigIntegerAlgorithms.hh"
#include "BigInteger\BigUnsigned.hh"
#include "Session.h"
#include <utility>

class Session
{
protected:
	BigUnsigned p;
	BigUnsigned g;
	BigUnsigned x;
	BigUnsigned y;
	BigUnsigned k;

public:
	Session(BigUnsigned p, BigUnsigned g, BigUnsigned x, BigUnsigned y, BigUnsigned k);

	std::pair<BigUnsigned, BigUnsigned> encrypt(BigUnsigned block);
};

