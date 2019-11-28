#include <windows.h> 
#include <iostream>
#include <vector>
#include "stdlib.h"
#include "vector"
#include "string"
#include <fstream>
#include <string>
#include <streambuf>

#include "RSA_realize.h"

using namespace std;

BigUnsigned convert_bytes_to_big_unsigned(string block) {
	BigUnsigned result = 0;

	for (int i = 0; i < block.length(); ++i) {
		result = (result << 8);

		BigUnsigned ch = static_cast<unsigned char>(block[i]);
		result = result | ch;
	}
	return result;
}


string convert_big_unsigned_to_bytes(BigUnsigned block) {
	string result;
	BigUnsigned mask = 0b11111111;

	while (block != 0) {
		char next_char = (block & mask).toInt();
		result += next_char;
		block = block >> 8;
	}

	reverse(result.begin(), result.end());

	return result;
}


string get_block(string message, unsigned int i, BigUnsigned block_size) {
	int size = block_size.toInt();
	return message.substr(size * i, size);
}


vector<BigUnsigned> get_big_unsigned_numbers(string encrypted_message) {
	int pos = 0;
	string token;
	BigUnsigned token_big_unsigned;
	vector<BigUnsigned> encrypted_blocks;

	while ((pos = encrypted_message.find(" ")) != string::npos) {
		token = encrypted_message.substr(0, pos);
		encrypted_message.erase(0, pos + 1);
		token_big_unsigned = stringToBigUnsigned(token);
		encrypted_blocks.push_back(token_big_unsigned);
	}

	return encrypted_blocks;
}


void encrypt(HWND hWnd, RSA rsa, string fileName) {
	unsigned int block_size = rsa.get_block_size();
	string ofn;
	string message = read_secret_file(fileName);
	string encrypted_message = "";

	unsigned int blocks_number = message.length() % block_size /* here was 8 */ == 0
		? message.length() / block_size
		: message.length() / block_size + 1;

	for (int i = 0; i < blocks_number; ++i) {
		string block = get_block(message, i, block_size);

		BigUnsigned num_block = convert_bytes_to_big_unsigned(block);
		BigUnsigned encrypted_num_block = rsa.encrypt(num_block);

		encrypted_message += bigUnsignedToString(encrypted_num_block);
		encrypted_message += " ";
	}

	if (ofn == "bad")
		return;
	ofn = OpenUserFile(hWnd);
	write_to_encrypted_file(hWnd, ofn, encrypted_message);
}


void decrypt(HWND hWnd, RSA rsa, string fileName) {
	unsigned int block_size = rsa.get_block_size();
	string ofn;
	string encrypted_message = read_encrypted_file(fileName);
	vector<BigUnsigned> blocks = get_big_unsigned_numbers(encrypted_message);
	string decrypted_message = "";

	for (int i = 0; i < blocks.size(); ++i) {
		BigUnsigned encrypted_num_block = blocks[i];
		BigUnsigned decrypted_num_block = rsa.decrypt(encrypted_num_block);
		std::string decrypted_block = convert_big_unsigned_to_bytes(decrypted_num_block);
		decrypted_message += decrypted_block;
	}

	ofn = OpenUserFile(hWnd);
	if (ofn == "bad")
		return;
	write_to_decrypted_file(hWnd, ofn, decrypted_message);
}

// RSA-155 (512 bits)
const string RSA_512_P = "102639592829741105772054196573991675900716567808038066803341933521790711307779";
const string RSA_512_Q = "106603488380168454820927220360012878679207958575989291522270608237193062808643";


int mainRSA(HWND hWnd, bool ENCRYPT) {
	BigUnsigned p = stringToBigUnsigned(RSA_512_P);				// 169691;
	BigUnsigned q = stringToBigUnsigned(RSA_512_Q);				// 786431;
	string filename;

	filename = OpenUserFile(hWnd);
	if (filename == "bad")
		return 1;

	auto rsa = RSA(p, q);

	if (ENCRYPT) {
		encrypt(hWnd, rsa, filename);
	}

	if (!ENCRYPT) {
		decrypt(hWnd, rsa, filename);
	}
	return 0;
}