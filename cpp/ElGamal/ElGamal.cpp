#include "ElGamal.h"

ElGamal::ElGamal(BigUnsigned p, BigUnsigned g, BigUnsigned x, BigUnsigned y) {
	this->p = p;
	this->g = g;
	this->x = x;
	this->y = y;
};

ElGamal::ElGamal(BigUnsigned p) {
	BigUnsigned phi = p - 1;
	BigUnsigned g = 0;
		
	BigUnsigned d = phi / 2;
	for (BigUnsigned i = 0; true; i++) {
		if (modexp(i, d, p) == phi) {
			g = i;
			break;
		} // Algo from studfiles
	}

	// auto modulo = modexp(g, phi, p).toInt();

	BigUnsigned x = 0;
	
	while (x < p) {
		x += (unsigned int)rand();
		x = x << 32;
	}

	x = x >> 32;
	
	BigUnsigned y = modexp(g, x, p);

	this->p = p;
	this->g = g;
	this->x = x;
	this->y = y;
}

unsigned int ElGamal::get_block_size() {
	unsigned int size_bytes = 2;
	BigUnsigned size = 65536;

	BigUnsigned modulo = this->p;

	while (size < modulo) {
		size *= size;
		size_bytes *= 2;
	}

	return size_bytes / 2;
}

Session ElGamal::get_session() {
	BigUnsigned k = 0;

	while (k < p) {
		k += (unsigned int)rand();
		k = k << 32;
	}

	k = k >> 32;

	return Session(p, g, x, y, k);
};

Decryptor ElGamal::get_decryptor(BigUnsigned a, BigUnsigned b) {
	return Decryptor(p, x, a, b);
}