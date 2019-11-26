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

#include "IOCommon.h"
#include "RSA.h"

using namespace std;

BigUnsigned convert_bytes_to_big_unsigned(string block);

string convert_big_unsigned_to_bytes(BigUnsigned block);
string get_block(string message, unsigned int i, BigUnsigned block_size);
vector<BigUnsigned> get_big_unsigned_numbers(string encrypted_message);

void encrypt(HWND hWnd, RSA rsa, string filename);
void decrypt(HWND hWnd, RSA rsa, string filename);

int mainRSA(HWND hWnd, bool ENCRYPT);