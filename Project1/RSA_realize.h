#pragma once
#include "BigInteger\BigIntegerLibrary.hh"
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

std::string OpenUserFile(HWND hWnd);
BigUnsigned convert_bytes_to_big_unsigned(std::string block);

std::string convert_big_unsigned_to_bytes(BigUnsigned block);
std::string get_block(std::string message, unsigned int i, BigUnsigned block_size);
std::vector<BigUnsigned> get_big_unsigned_numbers(std::string encrypted_message);

void encrypt(HWND hWnd, RSA rsa, std::string fileName);
void decrypt(HWND hWnd, RSA rsa, std::string fileName);

int mainRSA(HWND hWnd, std::string fileName);