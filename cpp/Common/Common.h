#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>


std::string SECRET_FILE = "../secret.txt";
std::string ENCRYPTED_FILE = "../encrypted.txt";
std::string DECRYPTED_FILE = "../decrypted.txt";


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

void write_to_file(std::string filename, std::string output) {
	std::ofstream output_stream;
	output_stream.open(filename, std::ios::out | std::ios::binary);
	output_stream << output;
	output_stream.close();
}

std::string read_secret_file() {
	return read_file(SECRET_FILE);
}

void write_to_encrypted_file(std::string output) {
	write_to_file(ENCRYPTED_FILE, output);
}

std::string read_encrypted_file() {
	return read_file(ENCRYPTED_FILE);
}

void write_to_decrypted_file(std::string output) {
	write_to_file(DECRYPTED_FILE, output);
}