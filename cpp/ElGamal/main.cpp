#include <iostream>
#include <vector>
#include "BigInteger\BigUnsigned.hh"
#include "BigInteger\BigIntegerUtils.hh"
#include "BigInteger\BigIntegerAlgorithms.hh"
#include "..//Common/Common.h"
#include "ElGamal.h"


BigUnsigned convert_bytes_to_big_unsigned(std::string block) {
	BigUnsigned result = 0;

	for (int i = 0; i < block.length(); ++i) {
		result = (result << 8);

		BigUnsigned ch = block[i];
		result = result | ch;
	}

	return result;
}


std::string convert_big_unsigned_to_bytes(BigUnsigned block) {
	std::string result;
	BigUnsigned mask = 0b11111111;

	while (block != 0) {
		char next_char = (block & mask).toInt();
		result += next_char;
		block = block >> 8;
	}

	std::reverse(result.begin(), result.end());

	return result;
}


std::string get_block(std::string message, unsigned int i, BigUnsigned block_size) {
	int size = block_size.toInt();
	return message.substr(size * i, size);
}


std::vector<BigUnsigned> get_big_unsigned_numbers(std::string encrypted_message) {
	int pos = 0;
	std::string token;
	BigUnsigned token_big_unsigned;
	std::vector<BigUnsigned> encrypted_blocks;

	while ((pos = encrypted_message.find(" ")) != std::string::npos) {
		token = encrypted_message.substr(0, pos);
		encrypted_message.erase(0, pos + 1);
		token_big_unsigned = stringToBigUnsigned(token);
		encrypted_blocks.push_back(token_big_unsigned);
	}

	return encrypted_blocks;
}


void encrypt(ElGamal cypher) {
	unsigned int block_size = cypher.get_block_size();
	Session session = cypher.get_session();

	std::string message = read_secret_file();
	std::string encrypted_message = "";

	unsigned int blocks_number = message.length() % block_size == 0
		? message.length() / block_size
		: message.length() / block_size + 1;

	for (int i = 0; i < blocks_number; ++i) {
		std::string block = get_block(message, i, block_size);

		BigUnsigned num_block = convert_bytes_to_big_unsigned(block);
		std::pair<BigUnsigned, BigUnsigned> encrypted_num_blocks = session.encrypt(num_block);

		encrypted_message += bigUnsignedToString(encrypted_num_blocks.first);
		encrypted_message += " ";
		encrypted_message += bigUnsignedToString(encrypted_num_blocks.second);
		encrypted_message += " ";
	}

	write_to_encrypted_file(encrypted_message);
}


void decrypt(ElGamal cypher) {
	unsigned int block_size = cypher.get_block_size();

	std::string encrypted_message = read_encrypted_file();
	std::vector<BigUnsigned> blocks = get_big_unsigned_numbers(encrypted_message);

	std::string decrypted_message = "";

	for (int i = 0; i < blocks.size(); i += 2) {
		BigUnsigned a = blocks[i];
		BigUnsigned b = blocks[i + 1];

		std::string a_string = bigIntegerToString(a);
		std::string b_string = bigIntegerToString(b);

		Decryptor decryptor = cypher.get_decryptor(a, b);

		BigUnsigned decrypted_num_block = decryptor.decrypt();
		decrypted_message += convert_big_unsigned_to_bytes(decrypted_num_block);
	}

	write_to_decrypted_file(decrypted_message);
}


int main()
{
	srand(11092001);

	BigUnsigned p = stringToBigUnsigned("1634733645809253848443133883865090859841783670033092312181110852389333100104508151212118167511579");
	ElGamal cypher = ElGamal(p);

	encrypt(cypher);
	decrypt(cypher);
}
