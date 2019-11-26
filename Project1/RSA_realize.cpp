#include <windows.h> 
#include <iostream>
#include <vector>
#include "stdlib.h"
#include "vector"
#include "string"
#include <fstream>
#include <string>
#include <streambuf>

#include "RSA.h"
#include "RSA_realize.h"
#include "IOCommon.h"

std::string OpenUserFile(HWND hWnd) {
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[150];       // buffer for file name
	HANDLE hf;              // file handle
	wchar_t file_path[150];
	//std::string strr;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All files\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	
	//std::string flname = GetOpenFileName(&ofn);
	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn) == TRUE) {
		/*hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
			*/
		mbstowcs(file_path, szFile, 150);
		std::wstring ws(file_path);
		// your new String
		std::string strr(ws.begin(), ws.end());
		return strr;
	}
	return FALSE;
}

BigUnsigned convert_bytes_to_big_unsigned(std::string block) {
	BigUnsigned result = 0;

	for (int i = 0; i < block.length(); ++i) {
		result = (result << 8);

		BigUnsigned ch = static_cast<unsigned char>(block[i]);
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


void encrypt(HWND hWnd, RSA rsa, std::string fileName) {
	unsigned int block_size = rsa.get_block_size();
	std::string ofn;
	std::string message = read_secret_file(fileName);
	std::string encrypted_message = "";

	unsigned int blocks_number = message.length() % block_size /* Çהוסü בûכמ 8 */ == 0
		? message.length() / block_size
		: message.length() / block_size + 1;

	for (int i = 0; i < blocks_number; ++i) {
		std::string block = get_block(message, i, block_size);

		BigUnsigned num_block = convert_bytes_to_big_unsigned(block);
		BigUnsigned encrypted_num_block = rsa.encrypt(num_block);

		encrypted_message += bigUnsignedToString(encrypted_num_block);
		encrypted_message += " ";
	}

	ofn = OpenUserFile(hWnd);
	write_to_encrypted_file(hWnd, ofn, encrypted_message);
	//CloseHandle(ofn);;
}


void decrypt(HWND hWnd, RSA rsa, std::string fileName) {
	unsigned int block_size = rsa.get_block_size();
	std::string ofn;
	std::string encrypted_message = read_encrypted_file(fileName);
	std::vector<BigUnsigned> blocks = get_big_unsigned_numbers(encrypted_message);
	std::string decrypted_message = "";

	for (int i = 0; i < blocks.size(); ++i) {
		BigUnsigned encrypted_num_block = blocks[i];
		BigUnsigned decrypted_num_block = rsa.decrypt(encrypted_num_block);
		std::string decrypted_block = convert_big_unsigned_to_bytes(decrypted_num_block);
		decrypted_message += decrypted_block;
	}

	ofn = OpenUserFile(hWnd);
	write_to_decrypted_file(hWnd, decrypted_message, ofn);
}

// RSA-155 (512 bits)
const std::string RSA_512_P = "102639592829741105772054196573991675900716567808038066803341933521790711307779";
const std::string RSA_512_Q = "106603488380168454820927220360012878679207958575989291522270608237193062808643";


//const bool ENCRYPT = true;
//const bool DECRYPT = true;

int mainRSA(HWND hWnd, std::string fileName, bool ENCRYPT) {
	BigUnsigned p = stringToBigUnsigned(RSA_512_P); // 169691;
	BigUnsigned q = stringToBigUnsigned(RSA_512_Q); // 786431;

	auto rsa = RSA(p, q);

	if (ENCRYPT) {
		encrypt(hWnd, rsa, fileName);
	}

	if (!ENCRYPT) {
		decrypt(hWnd, rsa, fileName);
	}
	return 0;
}