#include <windows.h> 
#include <commctrl.h>
#include <Uxtheme.h>
#include "commdlg.h"
#include "stdlib.h"
#include "vector"
#include "string"
#include <fstream>
#include <iostream>
#include <string>
#include <streambuf>

#include "RSA_realize.h"
#pragma comment (lib, "comctl32.lib")

#define MAX_LOADSTRING 100
#define ID_BUTTON 101
#define ID_EDIT 102
#define _CRT_SECURE_NO_WARNINGS

#define ID_TOOLBAR 100
#define CM_FILE_NEW 1000   // Команда Создать.
#define CM_FILE_OPEN 1001  // Команда Открыть.
#define CM_FILE_SAVE 1002  // Команда Сохранить.
#define CM_FILE_QUIT 1003  // Команда Выход.
#define CM_EDIT_CUT 2000   // Команда Вырезать.
#define CM_EDIT_PASTE 2001 // Команда Вставить.
#define CM_EDIT_COPY 2002  // Команда Копировать.
#define CM_EDIT_DEL 2003   // Команда Удалить.
#define CM_HELP_HELP 3000  // Команда Справка.
#define CM_HELP_ABOUT 3001 // Команда О программе.

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
std::string read_file(std::string filename);

HINSTANCE hInstance;
TCHAR szClass[] = TEXT("MenuAndToolBarClass");

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {
	MSG msg;
	HWND hWnd;
	hInstance = hInst;

	if (!RegClass(WndProc, szClass, COLOR_WINDOW)) return FALSE;
	hWnd = CreateWindow(szClass, TEXT("CoDec"), 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 
			CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, NULL);

	if (!hWnd) return FALSE;
	SetWindowTheme(hWnd, L" ", L" "); // ADD THIS
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground) {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION + 5);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(135, 187, 163)); //(HBRUSH)(brBackground);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szName;

	return (RegisterClass(&wc) != 0);
}

BOOL CreateMenuItem(HMENU hMenu, TCHAR *str, UINT uIns, UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType) {
	MENUITEMINFO mii;

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mii.fType = fType;
	mii.fState = MFS_ENABLED;
	mii.dwTypeData = str;
	mii.cch = sizeof(str);
	mii.wID = uCom;
	mii.hSubMenu = hSubMenu;

	return InsertMenuItem(hMenu, uIns, flag, &mii);
}

HWND CreateToolBar(HWND hWnd, DWORD dwStyle, UINT uCom) {
	static TBBUTTON but[10];

	but[0].fsStyle = TBSTYLE_SEP;

	// Кнопка Создать.
	but[1].iBitmap = STD_FILENEW;
	but[1].idCommand = CM_FILE_NEW;
	but[1].fsState = TBSTATE_ENABLED;
	but[1].fsStyle = TBSTYLE_BUTTON;

	// Кнопка Открыть.
	but[2].iBitmap = STD_FILEOPEN;
	but[2].idCommand = CM_FILE_OPEN;
	but[2].fsState = TBSTATE_ENABLED;
	but[2].fsStyle = TBSTYLE_BUTTON;

	// Кнопка Сохранить.
	but[3].iBitmap = STD_FILESAVE;
	but[3].idCommand = CM_FILE_SAVE;
	but[3].fsState = TBSTATE_ENABLED;
	but[3].fsStyle = TBSTYLE_BUTTON;

	// Кнопка Вырезать.
	but[4].iBitmap = STD_CUT;
	but[4].idCommand = CM_EDIT_CUT;
	but[4].fsState = TBSTATE_ENABLED;
	but[4].fsStyle = TBSTYLE_BUTTON;

	// Кнопка Копировать.
	but[5].iBitmap = STD_COPY;
	but[5].idCommand = CM_EDIT_COPY;
	but[5].fsState = TBSTATE_ENABLED;
	but[5].fsStyle = TBSTYLE_BUTTON;

	// Кнопка Вставить.
	but[6].iBitmap = STD_PASTE;
	but[6].idCommand = CM_EDIT_PASTE;
	but[6].fsState = TBSTATE_ENABLED;
	but[6].fsStyle = TBSTYLE_BUTTON;

	// Кнопка Удалить.
	but[7].iBitmap = STD_DELETE;
	but[7].idCommand = CM_EDIT_DEL;
	but[7].fsState = TBSTATE_ENABLED;
	but[7].fsStyle = TBSTYLE_BUTTON;

	// Кнопка Справка.
	but[8].iBitmap = STD_HELP;
	but[8].idCommand = CM_HELP_HELP;
	but[8].fsState = TBSTATE_ENABLED;
	but[8].fsStyle = TBSTYLE_BUTTON;

	return CreateToolbarEx(hWnd, dwStyle, uCom, 0, 
			HINST_COMMCTRL, IDB_STD_LARGE_COLOR, 
			but, 9, 0, 0, 0, 0, sizeof(TBBUTTON));
}

//std::string display_file(char* path) {
//	FILE* file;
//	file = fopen(path, "w+b");
//
//	std::ifstream input_stream(path, std::ios::in | std::ios::binary);
//	std::string result;
//
//	while (true) {
//		char ch;
//		input_stream.get(ch);
//
//		if (input_stream.eof()) {
//			return result;
//		}
//
//		result = result + ch;
//	}
//
	/*fseek(file, 0, SEEK_END);
	int _size = ftell(file);
	rewind(file);
	char* data = new char(_size+1);
	fread(data, _size, 1, file);
	data[_size] = '\0';*/
	////SetWindowText(hEdit, data)
	//fclose(file);
	//MessageBox(NULL, TEXT("Создать."), TEXT(""), MB_OK);
//}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMainMenu, hPopUpFile, hPopUpEdit, hPopUpHelp;
	static HWND hToolbar;
	OPENFILENAME ofn;

	switch (msg) {
	case WM_SIZE:
		MoveWindow(hToolbar, 0, 0, 0, 0, TRUE);
		return 0;
		
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case CM_FILE_NEW:		// code			
			ofn = OpenUserFile(hWnd);
			mainRSA(hWnd, ofn.lpstrFile);
			return 0;

		case CM_FILE_OPEN:	// decrypto
			//ofn = OpenFile(hWnd);
			//display_file(ofn.lpstrFile);
			//STB stb = STB(key, synchro);
			//stb.decrypt_gamming();

			MessageBox(NULL, TEXT("RSA"), TEXT(""), MB_OK);
			return 0;

		case CM_FILE_SAVE:
			MessageBox(NULL, TEXT("Сохранить."), TEXT(""), MB_OK);
			return 0;

		case CM_EDIT_CUT:
			MessageBox(NULL, TEXT("Вырезать."), TEXT(""), MB_OK);
			return 0;

		case CM_EDIT_COPY:
			MessageBox(NULL, TEXT("Копировать."), TEXT(""), MB_OK);
			return 0;

		case CM_EDIT_PASTE:
			MessageBox(NULL, TEXT("Вставить."), TEXT(""), MB_OK);
			return 0;

		case CM_EDIT_DEL:
			MessageBox(NULL, TEXT("Удалить."), TEXT(""), MB_OK);
			return 0;

		case CM_HELP_HELP:
			MessageBox(NULL, TEXT("Справка."), TEXT(""), MB_OK);
			return 0;

		case CM_HELP_ABOUT:
			MessageBox(NULL, TEXT("О программе."), TEXT(""), MB_OK);
			return 0;

		case CM_FILE_QUIT:
			DestroyWindow(hWnd);
			return 0;
		}
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, msg, wParam, lParam);
}