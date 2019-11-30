#include <windows.h> 
#include <commctrl.h>
#include <Uxtheme.h>
#include "commdlg.h"
#include "stdlib.h"
#include "vector"
#include "string"
#include <fstream>
#include <iostream>
#include <streambuf>

#include "RSA_realize.h"
#include "DES_realize.h"
#include "IOCommon.h"
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "Winmm.lib")

using namespace std;

#define ID_TOOLBAR 100
#define FILE_CODE_RSA 10			// RSA CODE
#define FILE_DECODE_RSA 20			// RSA DECODE
#define FILE_CODE_DES 11			
#define CM_FILE_QUIT 1000			//exit
#define FILE_DECODE_DES 21			
#define FILE_CODE_3DES 12			
#define FILE_DECODE_3DES 22			
#define CM_HELP_ABOUT 301			// About
#define FBUTTON 2000
#define MUSIC_OF 2001				// for muzic

static int SCREEN_WIDTH = 820;
static int SCREEN_HEIGHT = 400;
int state = 0;

// global
HINSTANCE hInst;    
HWND hLabel;

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;
TCHAR szClass[] = TEXT("MainClass");
TCHAR lzClass[] = TEXT("BUTTON");
int labelMain = 20; 
PAINTSTRUCT ps;
HDC hdcMem;
HBITMAP hbmMem;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {
	MSG msg;
	HWND hWnd;
	hInstance = hInst;

	if (!RegClass(WndProc, szClass, COLOR_WINDOW)) {
		MessageBox(NULL, TEXT("Error!!!"), TEXT("Exc"), MB_OK);
		return FALSE;
	}

	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);			// разрешение экрана

	int x = (screen_rect.right - SCREEN_WIDTH) / 2;
	int y = (screen_rect.bottom - SCREEN_HEIGHT) / 2;	

	hWnd = CreateWindow(szClass, TEXT("CoDecer"), 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, 
		SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, hInstance, NULL);	

	HWND btnRSAC = CreateWindow("BUTTON", TEXT("RSA code"),
		WS_CHILD | WS_VISIBLE, 50, 200,
		100, 30, hWnd, (HMENU)FILE_CODE_RSA, hInstance, NULL);

	HWND btnRSAD = CreateWindow("BUTTON", TEXT("RSA decode"),
		WS_CHILD | WS_VISIBLE, 170, 200,
		100, 30, hWnd, (HMENU)FILE_DECODE_RSA, hInstance, NULL);

	HWND btnDESC = CreateWindow("BUTTON", TEXT("DES code"),
		WS_CHILD | WS_VISIBLE, 290, 200,
		100, 30, hWnd, (HMENU)FILE_CODE_RSA, hInstance, NULL);

	HWND btnDESD = CreateWindow("BUTTON", TEXT("DES decode"),
		WS_CHILD | WS_VISIBLE, 410, 200,
		100, 30, hWnd, (HMENU)FILE_DECODE_RSA, hInstance, NULL);

	HWND btn3DESC = CreateWindow("BUTTON", TEXT("3DES code"),
		WS_CHILD | WS_VISIBLE, 530, 200,
		100, 30, hWnd, (HMENU)FILE_CODE_RSA, hInstance, NULL);

	HWND btn3DESD = CreateWindow("BUTTON", TEXT("3DES decode"),
		WS_CHILD | WS_VISIBLE, 650, 200,
		100, 30, hWnd, (HMENU)FILE_DECODE_RSA, hInstance, NULL);

	HWND btnMusic = CreateWindow("BUTTON", TEXT("Music"),
		WS_CHILD | WS_VISIBLE, 350, 250,
		100, 30, hWnd, (HMENU)MUSIC_OF, hInstance, NULL);

	HWND btnExit = CreateWindow("BUTTON", TEXT("Exit"),
		WS_CHILD | WS_VISIBLE, 350, 300,
		100, 30, hWnd, (HMENU)CM_FILE_QUIT, hInstance, NULL);

	hLabel = CreateWindow("static", "Text",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		200, 100, 400, 30, hWnd, (HMENU)labelMain,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	SetWindowText(hLabel, "There are some algorythm of encrypting: RSA, DES, 3DES");

	if (!hWnd)
		return FALSE;
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

BOOL CreateMenuItem(HMENU hMenu, TCHAR *str, UINT uIns, 
	UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType) {
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

	but[1].iBitmap = STD_FILENEW;
	but[1].idCommand = FILE_CODE_RSA;
	but[1].fsState = TBSTATE_ENABLED;
	but[1].fsStyle = TBSTYLE_BUTTON;

	but[2].iBitmap = STD_FILEOPEN;
	but[2].idCommand = FILE_DECODE_RSA;
	but[2].fsState = TBSTATE_ENABLED;
	but[2].fsStyle = TBSTYLE_BUTTON;

	but[3].iBitmap = STD_FILENEW;
	but[3].idCommand = FILE_CODE_DES;
	but[3].fsState = TBSTATE_ENABLED;
	but[3].fsStyle = TBSTYLE_BUTTON;

	but[4].iBitmap = STD_FILEOPEN;
	but[4].idCommand = FILE_DECODE_DES;
	but[4].fsState = TBSTATE_ENABLED;
	but[4].fsStyle = TBSTYLE_BUTTON;

	but[5].iBitmap = STD_FILENEW;
	but[5].idCommand = FILE_CODE_3DES;
	but[5].fsState = TBSTATE_ENABLED;
	but[5].fsStyle = TBSTYLE_BUTTON;

	but[6].iBitmap = STD_FILEOPEN;
	but[6].idCommand = FILE_DECODE_3DES;
	but[6].fsState = TBSTATE_ENABLED;
	but[6].fsStyle = TBSTYLE_BUTTON;

	but[7].iBitmap = STD_DELETE;
	but[7].idCommand = CM_FILE_QUIT;
	but[7].fsState = TBSTATE_ENABLED;
	but[7].fsStyle = TBSTYLE_BUTTON;

	return CreateToolbarEx(hWnd, dwStyle, uCom, 0, 
			HINST_COMMCTRL, IDB_STD_LARGE_COLOR, 
			but, 8, 0, 0, 0, 0, sizeof(TBBUTTON));
}
///////////////////////////////////////////////////////////////////////////

void paintMenu(HDC hDC)
{
	SetTextColor(hDC, 0x00ff00);
	SetBkMode(hDC, TRANSPARENT);		//фоновый режим смешивания контекста заданного устройства
	char playMusic[] = "Turn On music";
	char knowText[] = "And smile";
	HFONT hf = CreateFont(40, 0, 0,		// угол наклона
		0, FW_REGULAR, 0, 0, 0, 0, 0, 0, 0, 0,
		"Times New Roman");
	HFONT hOldFont = (HFONT)SelectObject(hDC, hf);
	TextOut(hDC, 310, 50, playMusic, strlen(playMusic));
	TextOut(hDC, 330, 150, knowText, strlen(knowText));
	SelectObject(hDC, hOldFont);
	DeleteObject(hf);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMainMenu, hPopUpCode, hPopUpDecode, hPopUpHelp;
	static HWND hToolbar;
	int temp;

	switch (msg) {
	case WM_SIZE:
		MoveWindow(hToolbar, 0, 0, 0, 0, TRUE);
		return 0;

	case WM_PAINT:
	{
		HDC hDC;
		hDC = BeginPaint(hWnd, &ps);
		hdcMem = CreateCompatibleDC(hDC);
		hbmMem = CreateCompatibleBitmap(hDC, 650, 500);
		SelectObject(hdcMem, hbmMem);
		switch (state)
		{
			case 0:
			{
				paintMenu(hDC);
				break;
			}
		}
		BitBlt(hdcMem, 0, 0, 650, 600, hDC, 0, 0, SRCCOPY);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps);
		break;
	}
	
	case WM_CREATE:
	{
		hMainMenu = CreateMenu();
		// for file coding
		hPopUpCode = CreatePopupMenu();
		int i = 0;
		CreateMenuItem(hPopUpCode, TEXT("RSA"), i++, FILE_CODE_RSA, NULL, FALSE, MFT_STRING);
		CreateMenuItem(hPopUpCode, TEXT("DES"), i++, FILE_CODE_DES, NULL, FALSE, MFT_STRING);
		CreateMenuItem(hPopUpCode, TEXT("3 DES"), i++, FILE_CODE_3DES, NULL, FALSE, MFT_STRING);
		CreateMenuItem(hPopUpCode, NULL, i++, 0, NULL, FALSE, MFT_SEPARATOR);
		CreateMenuItem(hPopUpCode, TEXT("Exit1"), i++, CM_FILE_QUIT, NULL, FALSE, MFT_STRING);

		// for file decoding
		hPopUpDecode = CreatePopupMenu();
		i = 0;
		CreateMenuItem(hPopUpDecode, TEXT("RSA"), i++, FILE_DECODE_RSA, NULL, FALSE, MFT_STRING);
		CreateMenuItem(hPopUpDecode, TEXT("DES"), i++, FILE_DECODE_DES, NULL, FALSE, MFT_STRING);
		CreateMenuItem(hPopUpDecode, TEXT("3DES"), i++, FILE_DECODE_3DES, NULL, FALSE, MFT_STRING);
		CreateMenuItem(hPopUpDecode, NULL, i++, 0, NULL, FALSE, MFT_SEPARATOR);
		CreateMenuItem(hPopUpDecode, TEXT("Exit2"), i++, CM_FILE_QUIT, NULL, FALSE, MFT_STRING);

		// help
		hPopUpHelp = CreatePopupMenu();
		i = 0;
		CreateMenuItem(hPopUpHelp, TEXT("About"), i++, CM_HELP_ABOUT, NULL, FALSE, MFT_STRING);		
		CreateMenuItem(hPopUpHelp, NULL, i++, 0, NULL, FALSE, MFT_SEPARATOR);
		CreateMenuItem(hPopUpHelp, TEXT("Exit3"), i++, CM_FILE_QUIT, NULL, FALSE, MFT_STRING);		

		i = 0;
		CreateMenuItem(hMainMenu, TEXT("Code File"), i++, 0, hPopUpCode, FALSE, MFT_STRING);
		
		CreateMenuItem(hMainMenu, TEXT("Decode File"), i++, 0, hPopUpDecode, FALSE, MFT_STRING);
		CreateMenuItem(hMainMenu, TEXT("Help"), i++, 0, hPopUpHelp, FALSE, MFT_STRING);
		
		SetMenu(hWnd, hMainMenu);
		DrawMenuBar(hWnd);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | WS_DLGFRAME;
		hToolbar = CreateToolBar(hWnd, dwStyle, ID_TOOLBAR);

		RECT screen_rect;
		//TCHAR szClass[] = TEXT("MainClass");
		//PlaySound((LPCSTR)"thunder.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

		return 0;
	}

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case MUSIC_OF:						// code		RSA				
			PlaySound((LPCSTR)"thunder.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			return 0;
		case FILE_CODE_RSA:						// code		RSA				
			temp = mainRSA(hWnd, true);
			if (temp != 1)				
				MessageBox(NULL, TEXT("RSA code"), TEXT("Completed"), MB_OK);
			return 0;
		case FILE_DECODE_RSA:
			// decrypto RSA			
			temp = mainRSA(hWnd, false);
			if (temp != 1)
				MessageBox(NULL, TEXT("RSA decode"), TEXT("Completed"), MB_OK);
			return 0;

		case FILE_CODE_DES:						// code		DES
			temp = mainDES(hWnd, true);
			if (temp != 1)
				MessageBox(NULL, TEXT("DES code"), TEXT("Completed"), MB_OK);
			return 0;

		case FILE_DECODE_DES:					// decrypto		DES
			temp = mainDES(hWnd, false);
			if (temp != 1)
				MessageBox(NULL, TEXT("DES decode"), TEXT("Completed"), MB_OK);
			return 0;

		case FILE_CODE_3DES:					// code		DES3
			temp = mainDES3(hWnd, true);
			if (temp != 1)
				MessageBox(NULL, TEXT("3 DES code"), TEXT("Completed"), MB_OK);
			return 0;

		case FILE_DECODE_3DES:					// decrypto		DES3
			temp = mainDES3(hWnd, false);
			if (temp != 1)
				MessageBox(NULL, TEXT("3 DES decode"), TEXT("Completed"), MB_OK);
			return 0;

		case CM_HELP_ABOUT:
			MessageBox(NULL, TEXT("Developer of program: Lizunova Iryna 751003. \n This program is designed to encrypt and decrypt files. \n To do this, select Encrypt/Decrypt and select a file from the directory."), TEXT("About"), MB_OK);
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