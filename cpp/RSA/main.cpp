﻿#include <iostream>
#include <vector>
#include "RSA.h"
#include "..//Common/Common.h"


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


void encrypt(RSA rsa) {
	unsigned int block_size = rsa.get_block_size();

	std::string message = read_secret_file(); 
	std::string encrypted_message = "";

	unsigned int blocks_number = message.length() % block_size /* Здесь было 8 */ == 0
		? message.length() / block_size
		: message.length() / block_size + 1;

	for (int i = 0; i < blocks_number; ++i) {
		std::string block = get_block(message, i, block_size);

		BigUnsigned num_block = convert_bytes_to_big_unsigned(block);
		BigUnsigned encrypted_num_block = rsa.encrypt(num_block);

		encrypted_message += bigUnsignedToString(encrypted_num_block);
		encrypted_message += " ";
	}

	write_to_encrypted_file(encrypted_message);
}


void decrypt(RSA rsa) {
	unsigned int block_size = rsa.get_block_size();

	std::string encrypted_message = read_encrypted_file();
	std::vector<BigUnsigned> blocks = get_big_unsigned_numbers(encrypted_message);

	std::string decrypted_message = "";

	for (int i = 0; i < blocks.size(); ++i) {
		BigUnsigned encrypted_num_block = blocks[i];
		BigUnsigned decrypted_num_block = rsa.decrypt(encrypted_num_block);
		std::string decrypted_block = convert_big_unsigned_to_bytes(decrypted_num_block);
		decrypted_message += decrypted_block;
	}

	write_to_decrypted_file(decrypted_message);
}


// RSA-155 (512 bits)
const std::string RSA_512_P = "102639592829741105772054196573991675900716567808038066803341933521790711307779";
const std::string RSA_512_Q = "106603488380168454820927220360012878679207958575989291522270608237193062808643";


const bool ENCRYPT = true;
const bool DECRYPT = true;


int main()
{
	BigUnsigned p = stringToBigUnsigned(RSA_512_P); // 169691;
	BigUnsigned q = stringToBigUnsigned(RSA_512_Q); // 786431;

	auto rsa = RSA(p, q);

	if (ENCRYPT) {
		encrypt(rsa);
	}

	if (DECRYPT) {
		decrypt(rsa);
	}

	

	return 0;
}