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

using namespace std;

string OpenUserFile(HWND hWnd) {
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[150];       // buffer for file name
	wchar_t file_path[150];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All files\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;

	if (GetOpenFileName(&ofn) == TRUE) {
		mbstowcs(file_path, szFile, 150);
		wstring ws(file_path);
		string strr(ws.begin(), ws.end());
		return strr;
	}
	MessageBox(NULL, TEXT("OOOOpss!U didn't choose file"), TEXT("Completed"), MB_OK);
	return "bad";
}

string read_file(string filename) {
	size_t index = 0;
	while (true) {
		index = filename.find("\\", index);
		if (index == string::npos) 
			break;
		filename.replace(index, 1, "/");
		index += 1;
	}

	//string filename1 = "D:\\progr/deletee/Project1/new.txt";
	ifstream input_stream(filename, ios::in | ios::binary | ios::ate);
	string result;

	while (true) {
		char ch;
		input_stream.get(ch);

		if (input_stream.eof()) {
			return result;
		}

		result = result + ch;
	}
}

void write_to_file(HWND hWnd, string filename, string output) {
	ofstream output_stream;
	output_stream.open(filename, ios::out | ios::binary);
	output_stream << output;
	output_stream.close();
}

string read_secret_file(string SECRET_FILE) {
	return read_file(SECRET_FILE);
}

void write_to_encrypted_file(HWND hWnd, string ENCRYPTED_FILE, string output) {
	write_to_file(hWnd, ENCRYPTED_FILE, output);
}

string read_encrypted_file(string ENCRYPTED_FILE) {
	return read_file(ENCRYPTED_FILE);
}

void write_to_decrypted_file(HWND hWnd, string DECRYPTED_FILE, string output) {
	write_to_file(hWnd, DECRYPTED_FILE, output);
}