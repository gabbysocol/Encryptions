#pragma once

#include <string>

class STB {
protected:
	unsigned char* keyenc;
	unsigned char* synhro;

public:
	void encrypt_plain();
	void encrypt_block();
	void encrypt_counter();
	void encrypt_gamming();

	void decrypt_plain();
	void decrypt_block();
	void decrypt_counter();
	void decrypt_gamming();

	STB(const std::string keyenc, const std::string synhro);
};