#include "Session.h"

Session::Session(BigUnsigned p, BigUnsigned g, BigUnsigned x, BigUnsigned y, BigUnsigned k) {
	this->p = p;
	this->g = g;
	this->x = x;
	this->y = y;
	this->k = k;
}

std::pair<BigUnsigned, BigUnsigned> Session::encrypt(BigUnsigned block) {
	BigUnsigned a = modexp(g, k, p);
	BigUnsigned b = modexp(y, k, p) * block % p;

	return std::pair<BigUnsigned, BigUnsigned>(a, b);
};