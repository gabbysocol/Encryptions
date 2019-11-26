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

#include "DES.h"
#include "DES3.h"
#include "IOCommon.h"

using namespace std;

unsigned long long convert_bytes_to_ulong(string block);
string convert_ulong_to_bytes(unsigned long long block);

string get_block(string message, unsigned int i);
unsigned int get_blocks_count(string message); 

string align(string message);

void encrypt(HWND hWnd, string filename, DES des);
void decrypt(HWND hWnd, string filename, DES des);

void encrypt(HWND hWnd, string filename, DES3 des);
void decrypt(HWND hWnd, string filename, DES3 des);

int mainDES(HWND hWnd, bool ENCRYPT);
int mainDES3(HWND hWnd, bool ENCRYPT);