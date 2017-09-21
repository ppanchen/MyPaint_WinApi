#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include <vector>
#include <string>
#include <cstring>
#include <CommCtrl.h>
#include <iostream>

class Paint
{
	enum Consts {				//constants
		ID_STRAIGHT_LINE,
		ID_LINE,
		ID_RECTANGLE,
		ID_ELLIPSE,
		ID_COLOR,
		ID_BGCOLOR,
		ID_TRACKBAR,
		ID_CLRBUTTON
	};

	enum Colors {				// color constants
		RED,
		GREEN,
		BLUE,
		BLACK,
		WHITE
	};


	struct					Figure;		//for figures saving
	std::vector <Figure>	Figures;

	COLORREF				_color;  //pen settings
	int						_width;
	HPEN					_hPen;

	COLORREF				_bgColor;	//brush settings
	HBRUSH					_hBrush;

	int						_action;
	HWND					_hwnd;
	HWND					_paintWindow;

	int						_x, _y, _lastx, _lasty; //coordinates for painting


	LRESULT CALLBACK		RealWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK		RealWndPaintProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	ATOM					RegMyWindowClass(HINSTANCE hInstance, LPCTSTR lpzClassName, WNDPROC proc);

	void					PaintEllipse(HDC hdc);
	void					PaintLine(HDC hdc);
	void					PaintRectangle(HDC hdc);

	void					RepaintFigures(HDC hdc);
	void					AddNewFigure();
	void					CreateMyWindow(HINSTANCE hInstance, int iCmdShow, const char *szAppName, const char *szPaintAreaName);

public:
	Paint(HINSTANCE hInstance, int iCmdShow, const char *szAppName, const char *szPaintAreaName);
	~Paint();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK	WndPaintProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
