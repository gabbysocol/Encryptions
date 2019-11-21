#include "Decryptor.h"
Decryptor::Decryptor(BigUnsigned p, BigUnsigned x, BigUnsigned a, BigUnsigned b) {
	this->p = p;
	this->x = x;
	this->a = a;
	this->b = b;
};

BigUnsigned Decryptor::decrypt() {
	return modexp(a, p - 1 - x, p) * b % p;
};