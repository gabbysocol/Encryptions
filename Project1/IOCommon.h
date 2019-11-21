#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

std::string read_file(std::string filename) {
	std::ifstream input_stream(filename, std::ios::in | std::ios::binary);

	std::string result;

	while (true) {
		char ch;
		input_stream.get(ch);

		if (input_stream.eof()) {
			return result;
		}

		result = result + ch;
	}
}

void write_to_file(HWND hWnd, std::string filename, std::string output) {
	std::ofstream output_stream;
	output_stream.open(filename, std::ios::out | std::ios::binary);
	output_stream << output;
	output_stream.close();
}

std::string read_secret_file(std::string SECRET_FILE){
	return read_file(SECRET_FILE);
}

void write_to_encrypted_file(HWND hWnd, std::string ENCRYPTED_FILE, std::string output) {
	write_to_file(hWnd, ENCRYPTED_FILE, output);
}

std::string read_encrypted_file(std::string ENCRYPTED_FILE) {
	return read_file(ENCRYPTED_FILE);
}

void write_to_decrypted_file(HWND hWnd, std::string DECRYPTED_FILE, std::string output) {
	write_to_file(hWnd, DECRYPTED_FILE, output);
}