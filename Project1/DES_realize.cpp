#include <windows.h> 
#include <iostream>
#include <vector>
#include "stdlib.h"
#include "vector"
#include "string"
#include <fstream>
#include <string>
#include <streambuf>

#include "DES_realize.h"

using namespace std;

unsigned long long convert_bytes_to_ulong(string block) {
	unsigned long long result = 0;

	for (int i = 0; i < 8; ++i) {
		result = result << 8;

		unsigned char next_char = i < block.length() ? block[i] : 0;
		result = result | next_char;
	}

	return result;
}

string convert_ulong_to_bytes(unsigned long long block) {
	string result(8, '\0');

	for (int i = 0; i < 8; ++i) {
		unsigned char next_char = (unsigned char)block;
		result[8 - i - 1] = next_char;
		block = block >> 8;
	}

	return result;
}

string get_block(string message, unsigned int i) {
	return message.substr(8 * i, 8);
}

unsigned int get_blocks_count(string message) {
	return message.length() % 8 == 0
		? message.length() / 8
		: message.length() / 8 + 1;
}

string align(string message) {
	if (message.length() % 8 == 0) {
		return message;
	}
	else {
		int align_count = 8 - message.length() % 8;
		string aligner(align_count, ' ');
		return message + aligner;
	}
}


void encrypt(HWND hWnd, string fileName, DES des) {
	string raw_message = read_secret_file(fileName);
	string message = align(raw_message);
	string encrypted_message = "";
	string ofn;

	unsigned int blocks_count = get_blocks_count(message);

	for (unsigned int i = 0; i < blocks_count; ++i) {
		string string_block = get_block(message, i);
		unsigned long long long_block = convert_bytes_to_ulong(string_block);
		unsigned long long encrypted_block = des.encrypt(long_block);
		encrypted_message += convert_ulong_to_bytes(encrypted_block);
	}

	ofn = OpenUserFile(hWnd);
	if (ofn == "bad")
		return;
	write_to_encrypted_file(hWnd, ofn, encrypted_message);
}


void decrypt(HWND hWnd, string fileName, DES des) {	
	string encrypted_message = read_encrypted_file(fileName);
	string decrypted_message = "";
	string ofn;

	unsigned int blocks_count = get_blocks_count(encrypted_message);

	for (unsigned int i = 0; i < blocks_count; ++i) {
		string encrypted_string_block = get_block(encrypted_message, i);
		unsigned long long encrypted_long_block = convert_bytes_to_ulong(encrypted_string_block);
		unsigned long long decrypted_block = des.decrypt(encrypted_long_block);
		decrypted_message += convert_ulong_to_bytes(decrypted_block);
	}

	//cout << decrypted_message << endl << endl;
	ofn = OpenUserFile(hWnd);
	if (ofn == "bad")
		return;
	write_to_decrypted_file(hWnd, ofn, decrypted_message);
}

void encrypt(HWND hWnd, string fileName, DES3 des) {
	string raw_message = read_secret_file(fileName);
	string message = align(raw_message);
	string encrypted_message = "";
	string ofn;

	unsigned int blocks_count = get_blocks_count(message);

	for (unsigned int i = 0; i < blocks_count; ++i) {
		string string_block = get_block(message, i);
		unsigned long long long_block = convert_bytes_to_ulong(string_block);
		unsigned long long encrypted_block = des.encrypt(long_block);
		encrypted_message += convert_ulong_to_bytes(encrypted_block);
	}

	ofn = OpenUserFile(hWnd);
	if (ofn == "bad")
		return;
	write_to_encrypted_file(hWnd, ofn, encrypted_message);
}

void decrypt(HWND hWnd, string fileName, DES3 des) {
	string encrypted_message = read_encrypted_file(fileName);
	string decrypted_message = "";
	string ofn;

	unsigned int blocks_count = get_blocks_count(encrypted_message);

	for (unsigned int i = 0; i < blocks_count; ++i) {
		string encrypted_string_block = get_block(encrypted_message, i);
		unsigned long long encrypted_long_block = convert_bytes_to_ulong(encrypted_string_block);
		unsigned long long decrypted_block = des.decrypt(encrypted_long_block);
		decrypted_message += convert_ulong_to_bytes(decrypted_block);
	}

	ofn = OpenUserFile(hWnd);
	if (ofn == "bad")
		return;
	write_to_decrypted_file(hWnd, ofn, decrypted_message);
}

const char* SECRET_KEY = "mysecretkey";
const char* SECRET_KEY_DES2 = "12345678";
const char* SECRET_KEY_DES3 = "32132352";

int mainDES3(HWND hWnd, bool ENCRYPT) {
	unsigned long long key = convert_bytes_to_ulong(SECRET_KEY);
	unsigned long long key2 = convert_bytes_to_ulong(SECRET_KEY_DES2);
	unsigned long long key3 = convert_bytes_to_ulong(SECRET_KEY_DES3);
	string filename;

	filename = OpenUserFile(hWnd);
	if (filename == "bad")
		return 1;
	DES3 des3_encryptor(key, key2, key3);

	if (ENCRYPT) {
		encrypt(hWnd, filename, des3_encryptor);
	}

	DES3 des3_decryptor(key, key2, key3);

	if (!ENCRYPT) {
		decrypt(hWnd, filename, des3_decryptor);
	}

	return 0;
}

int mainDES(HWND hWnd, bool ENCRYPT) {
	unsigned long long key = convert_bytes_to_ulong(SECRET_KEY);

	string filename;

	filename = OpenUserFile(hWnd);
	if (filename == "bad")
		return 1;
	filename = OpenUserFile(hWnd);
	DES des_encryptor(key);

	if (ENCRYPT) {
		encrypt(hWnd, filename, des_encryptor);
	}

	DES des_decryptor(key);	

	if (!ENCRYPT) {
		decrypt(hWnd, filename, des_decryptor);
	}

	return 0;
}