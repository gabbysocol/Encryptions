#pragma once

#include "BigInteger\BigUnsigned.hh"
#include "Session.h"
#include <utility>

class Decryptor
{
protected:
	BigUnsigned p;
	BigUnsigned x;
	BigUnsigned a;
	BigUnsigned b;

public:
	Decryptor(BigUnsigned p, BigUnsigned x, BigUnsigned a, BigUnsigned b);
	BigUnsigned decrypt();
};

