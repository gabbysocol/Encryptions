#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstring>

using namespace std;

string OpenUserFile(HWND hWnd);

string read_file(string filename);

void write_to_file(HWND hWnd, string filename, string output);

string read_secret_file(string SECRET_FILE);

void write_to_encrypted_file(HWND hWnd, string ENCRYPTED_FILE, string output);

string read_encrypted_file(string ENCRYPTED_FILE);

void write_to_decrypted_file(HWND hWnd, string DECRYPTED_FILE, string output);