#include "RSA.h"
#include "BigInteger\BigIntegerLibrary.hh"
#include "BigInteger\BigIntegerUtils.hh"

using namespace std;

BigUnsigned RSA::gcd_extended(BigUnsigned a, BigUnsigned b, BigInteger& x, BigInteger& y) {
	if (a == 0) {
		x = 0; 
		y = 1;
		return b;
	}

	BigInteger x1, y1;
	BigUnsigned d = gcd_extended(b % a, a, x1, y1);

	x = y1 - (BigInteger)(b / a) * x1;
	y = x1;

	return d;
}

unsigned int RSA::get_block_size() {
	unsigned int size_bytes = 2;
	BigUnsigned size = 65536;

	BigUnsigned modulo = public_key.PQ;

	while (size < modulo) {
		size *= size;
		size_bytes *= 2;
	}

	return size_bytes / 2;
}

BigUnsigned RSA::encrypt(BigUnsigned block) {
	return modexp(block, public_key.E, public_key.PQ);
}

BigUnsigned RSA::decrypt(BigUnsigned block) {
	return modexp(block, private_key.D, private_key.PQ);
}

RSA::RSA(BigUnsigned p, BigUnsigned q) {
	BigUnsigned n = p * q;

	BigUnsigned t = (p - 1) * (q - 1);

	BigUnsigned e = 65537;  // Recommended practice

	while (gcd(e, t) != 1) {
		e += 2;
	}

	BigInteger d_int = 0;
	BigInteger unused = 0;
	gcd_extended(e, t, d_int, unused);

	while (d_int < 0) {
		d_int += t;
	}

	BigUnsigned d = d_int.getMagnitude();

	std::cout << "e: " << bigIntegerToString(e) << std::endl << "d: " << bigIntegerToString(d_int) << std::endl;
	std::cout << "e * d % t = " << (e * d) % t << std::endl;

	this->private_key = PrivateKey();
	this->private_key.PQ = n;
	this->private_key.D = d;

	this->public_key = PublicKey();
	this->public_key.PQ = n;
	this->public_key.E = e;
}