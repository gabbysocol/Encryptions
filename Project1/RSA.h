#pragma once

#include "BigInteger\BigIntegerLibrary.hh"
#include "PrivateKey.h"
#include "PublicKey.h"

class RSA {
private:
	BigUnsigned gcd_extended(BigUnsigned a, BigUnsigned b, BigInteger& x, BigInteger& y);

	PrivateKey private_key;

public:

	unsigned int get_block_size();

	PublicKey public_key;

	BigUnsigned encrypt(BigUnsigned block);

	BigUnsigned decrypt(BigUnsigned block);

	RSA(BigUnsigned p, BigUnsigned q);
};

