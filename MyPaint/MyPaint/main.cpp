
#include <windows.h>
#include "Paint.h"

//debug------------------------------
//#include <fcntl.h>
//#include <io.h>
//#include <iostream>

//FILE *g_ic_file_cout_stream; FILE *g_ic_file_cin_stream;
//bool InitConsole()
//{
//	if (!AllocConsole()) { return false; }
//	if (freopen_s(&g_ic_file_cout_stream, "CONOUT$", "w", stdout) != 0) { return false; } // For std::cout 
//	if (freopen_s(&g_ic_file_cin_stream, "CONIN$", "w+", stdin) != 0) { return false; } // For std::cin
//	return true;
//}
//-----------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow) {
	Paint MyPaint(hInstance, iCmdShow, "MyPaint", "MyPaintArea");
	//if (InitConsole())
	//	std::cout << "Start" << std::endl;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))				//The Message Loop
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}