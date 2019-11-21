#pragma once

#include "BigInteger\BigIntegerAlgorithms.hh"
#include "BigInteger\BigUnsigned.hh"
#include "Decryptor.h"
#include "Session.h"
#include <cstdlib>

class ElGamal
{
protected:
	BigUnsigned p;
	BigUnsigned g;
	BigUnsigned x;
	BigUnsigned y;

public:
	ElGamal(BigUnsigned p, BigUnsigned g, BigUnsigned x, BigUnsigned y);
	ElGamal(BigUnsigned p);
	unsigned int get_block_size();
	Session get_session();
	Decryptor get_decryptor(BigUnsigned a, BigUnsigned b);
};

