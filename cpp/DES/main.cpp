#include <iostream>
#include "des.h"
#include "des3.h"
#include "..\Common\Common.h"

using namespace std;

/*
unsigned long long convert_bytes_to_ulong(string block) {
	unsigned long long result = 0;

	for (int i = 0; i < 8; ++i) {
		result = (result << 8);
		
		// If block didn't ended yet
		if (i < block.length()) {
			result = result | block[i];
		}
	}

	return result;
}

string convert_ulong_to_bytes(unsigned long long block) {
	char result[8];

	for (int i = 0; i < 8; ++i) {
		result[i] = 0;
	}

	for (int i = 0; i < 8; ++i) {
		char next_char = (char)block;
		result[8 - i - 1] = next_char;
		block = block >> 8;
	}

	return string(result, 8);
}
*/

unsigned long long convert_bytes_to_ulong(string block) {
	unsigned long long result = 0;

	for (int i = 0; i < 8; ++i) {
		result = result << 8;

		unsigned char next_char = i < block.length() 
			? block[i] : 0;

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


void encrypt(DES des) {
	string raw_message = read_secret_file();
	string message = align(raw_message);
	string encrypted_message = "";

	cout << message << endl << endl;

	unsigned int blocks_count = get_blocks_count(message);

	for (unsigned int i = 0; i < blocks_count; ++i) {
		string string_block = get_block(message, i);
		unsigned long long long_block = convert_bytes_to_ulong(string_block);
		unsigned long long encrypted_block = des.encrypt(long_block);
		encrypted_message += convert_ulong_to_bytes(encrypted_block);
	}

	cout << encrypted_message << endl << endl;
	write_to_encrypted_file(encrypted_message);
}


void decrypt(DES des) {
	string encrypted_message = read_encrypted_file();
	string decrypted_message = "";

	unsigned int blocks_count = get_blocks_count(encrypted_message);

	for (unsigned int i = 0; i < blocks_count; ++i) {
		string encrypted_string_block = get_block(encrypted_message, i);
		unsigned long long encrypted_long_block = convert_bytes_to_ulong(encrypted_string_block);
		unsigned long long decrypted_block = des.decrypt(encrypted_long_block);
		decrypted_message += convert_ulong_to_bytes(decrypted_block);
	}

	cout << decrypted_message << endl << endl;
	write_to_decrypted_file(decrypted_message);
}


void encrypt(DES3 des) {
	string raw_message = read_secret_file();
	string message = align(raw_message);
	string encrypted_message = "";

	cout << message << endl << endl;

	unsigned int blocks_count = get_blocks_count(message);

	for (unsigned int i = 0; i < blocks_count; ++i) {
		string string_block = get_block(message, i);
		unsigned long long long_block = convert_bytes_to_ulong(string_block);
		unsigned long long encrypted_block = des.encrypt(long_block);
		encrypted_message += convert_ulong_to_bytes(encrypted_block);
	}

	cout << encrypted_message << endl << endl;
	write_to_encrypted_file(encrypted_message);
}


void decrypt(DES3 des) {
	string encrypted_message = read_encrypted_file();
	string decrypted_message = "";

	unsigned int blocks_count = get_blocks_count(encrypted_message);

	for (unsigned int i = 0; i < blocks_count; ++i) {
		string encrypted_string_block = get_block(encrypted_message, i);
		unsigned long long encrypted_long_block = convert_bytes_to_ulong(encrypted_string_block);
		unsigned long long decrypted_block = des.decrypt(encrypted_long_block);
		decrypted_message += convert_ulong_to_bytes(decrypted_block);
	}

	cout << decrypted_message << endl << endl;
	write_to_decrypted_file(decrypted_message);
}


const bool ENCRYPT = true;
const bool DECRYPT = true;


const char* SECRET_KEY = "mysecretkey";
const char* SECRET_KEY_DES2 = "12345678";
const char* SECRET_KEY_DES3 = "32132352";

int main(int argc, char **argv)
{
	unsigned long long key = convert_bytes_to_ulong(SECRET_KEY);
	unsigned long long key2 = convert_bytes_to_ulong(SECRET_KEY_DES2);
	unsigned long long key3 = convert_bytes_to_ulong(SECRET_KEY_DES3);

	//DES des_encryptor(key);

	DES3 des3_encryptor(key, key2, key3);
	
	if (ENCRYPT) {
		encrypt(des3_encryptor);
	}

	//DES des_decryptor(key);

	DES3 des3_decryptor(key, key2, key3);

	if (DECRYPT) {
		decrypt(des3_decryptor);
	}

	return 0;
}
