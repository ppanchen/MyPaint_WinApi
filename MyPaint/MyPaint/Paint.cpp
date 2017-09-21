#include "Paint.h"


struct Paint::Figure {
	HBRUSH	hBrush;
	HPEN	hPen;
	int		x1, y1, x2, y2;
	int		type;
};

Paint::Paint(HINSTANCE hInstance, int iCmdShow, const char *szAppName, const char *szPaintAreaName){
	
	CreateMyWindow(hInstance, iCmdShow, szAppName, szPaintAreaName);

	//set initial state
	_action = -1;
	_bgColor = RGB(255, 255, 255);
	_color = RGB(0, 0, 0);
	_width = 6;
	_hPen = CreatePen(PS_SOLID, _width, _color);
	_hBrush = CreateSolidBrush(_bgColor);



	ShowWindow(_hwnd, iCmdShow);

	SetWindowLongPtr(_hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(this));
	SetWindowLongPtr(_paintWindow, GWLP_USERDATA, reinterpret_cast<LONG>(this));

}


Paint::~Paint(){
	return;
}

void Paint::CreateMyWindow(HINSTANCE hInstance, int iCmdShow, const char *szAppName, const char *szPaintAreaName) {
	TCHAR Colors[5][10] = {
		TEXT("Red"), TEXT("Green"), TEXT("Blue"), TEXT("Black"),
		TEXT("White")
	};
	if (!RegMyWindowClass(hInstance, szAppName, WndProc)) {
		MessageBox(NULL, TEXT("Error was detected on class registration"),
			szAppName, MB_ICONERROR);
		exit(1);
	}

	if (!RegMyWindowClass(hInstance, szPaintAreaName, WndPaintProc)) {
		MessageBox(NULL, TEXT("Error was detected on class registration"),
			szAppName, MB_ICONERROR);
		exit(1);
	}

	//creating window
	_hwnd = CreateWindow(szAppName, // window class name
		szAppName, // window caption
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // window style
		CW_USEDEFAULT,// initial x position
		CW_USEDEFAULT,// initial y position
		CW_USEDEFAULT,// initial x size
		CW_USEDEFAULT,// initial y size
		NULL,// parent window handle
		NULL,// window menu handle
		hInstance, // program instance handle
		NULL);

	CreateWindowEx(WS_EX_WINDOWEDGE,
		"BUTTON",
		"Straight line",
		WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // Styles 
		10, 20, 300, 20,
		_hwnd,
		(HMENU)ID_STRAIGHT_LINE,
		hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE,
		"BUTTON",
		"Line",
		WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
		10, 45, 300, 20,
		_hwnd,
		(HMENU)ID_LINE,
		hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE,
		"BUTTON",
		"Rectangle",
		WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
		10, 70, 300, 20,
		_hwnd,
		(HMENU)ID_RECTANGLE,
		hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE,
		"BUTTON",
		"Ellipse",
		WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
		10, 95, 300, 20,
		_hwnd,
		(HMENU)ID_ELLIPSE,
		hInstance, NULL);

	CreateWindow("static", "Border color:",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		10, 120, 140, 20,
		_hwnd,
		(HMENU)(501),
		hInstance, NULL);

	HWND hWndComboBoxColor = CreateWindowEx(WS_EX_WINDOWEDGE,
		"COMBOBOX",
		"Colors",
		WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST,
		160, 120, 150, 120,
		_hwnd,
		(HMENU)ID_COLOR,
		hInstance, NULL);
	CreateWindow("static", "Filling color:",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		10, 145, 140, 20,
		_hwnd,
		(HMENU)(501),
		hInstance, NULL);

	HWND hWndComboBoxBGColor = CreateWindowEx(WS_EX_WINDOWEDGE,
		"COMBOBOX",
		"Colors",
		WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST,
		160, 145, 150, 120,
		_hwnd,
		(HMENU)ID_BGCOLOR,
		hInstance, NULL);

	for (int k = 0; k < 5; k += 1) {
		// Add string to combobox.
		SendMessage(hWndComboBoxColor, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Colors[k]);
		SendMessage(hWndComboBoxBGColor, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Colors[k]);
	}
	// Send the CB_SETCURSEL message to display an initial item 
	//  in the selection field 
	SendMessage(hWndComboBoxColor, CB_SETCURSEL, (WPARAM)3, (LPARAM)0);
	SendMessage(hWndComboBoxBGColor, CB_SETCURSEL, (WPARAM)4, (LPARAM)0);

	CreateWindow("static", "Border width: ",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		10, 175, 140, 20,
		_hwnd,
		(HMENU)(501),
		hInstance, NULL);

	HWND hwndTrack = CreateWindowEx(
		0,                               // no extended styles 
		TRACKBAR_CLASS,                  // class name 
		"Trackbar Control",              // title (caption) 
		WS_CHILD |
		WS_VISIBLE |
		TBS_AUTOTICKS |
		TBS_ENABLESELRANGE,              // style 
		160, 170,                         // position 
		150, 30,                         // size 
		_hwnd,							 // parent window 
		(HMENU)ID_TRACKBAR,              // control identifier 
		hInstance,                       // instance 
		NULL                             // no WM_CREATE parameter 
	);

	SendMessage(hwndTrack, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(1, 11));		 // min. & max. positions
	SendMessage(hwndTrack, TBM_SETPAGESIZE, 0, (LPARAM)1);
	SendMessage(hwndTrack, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)6);
	SetFocus(hwndTrack);

	CreateWindowEx(WS_EX_WINDOWEDGE,
		"BUTTON",
		"Clear",
		WS_VISIBLE | WS_CHILD ,
		100, 210, 100, 30,
		_hwnd,
		(HMENU)ID_CLRBUTTON,
		hInstance, NULL);

	_paintWindow = CreateWindowEx(WS_EX_WINDOWEDGE,
		szPaintAreaName,
		"Paint area",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		370, 10,
		700, 500,
		_hwnd,
		NULL,
		hInstance, NULL);

}

ATOM Paint::RegMyWindowClass(HINSTANCE hInstance, LPCTSTR lpzClassName, WNDPROC proc)
{
	WNDCLASS wndclass = { 0 };
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = proc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpzClassName;
	return (RegisterClass(&wndclass));
}

LRESULT CALLBACK Paint::WndPaintProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Paint* me = reinterpret_cast<Paint*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (me) return me->RealWndPaintProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Paint::RealWndPaintProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static int start = 0;

	switch (message)
	{
	case WM_LBUTTONUP:			
		hdc = GetDC(hwnd);		
		switch (_action) {
		case ID_STRAIGHT_LINE:
			PaintLine(hdc);
			break;
		case ID_RECTANGLE:
			PaintRectangle(hdc);
			break;
		case ID_ELLIPSE:
			PaintEllipse(hdc);
			break;
		}
		ReleaseDC(hwnd, hdc);
		_lastx = 0;			
		_lasty = 0;
		return 0;
	case WM_LBUTTONDOWN:
		_lastx = LOWORD(lParam);						//Store the coordiantes
		_lasty = HIWORD(lParam);
		start = 1;
		return 0;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);								
		_x = LOWORD(lParam);							
		_y = HIWORD(lParam);							
		if (wParam & MK_LBUTTON & _action == ID_LINE)   
		{
				PaintLine(hdc);							//Draw the line frome the last pair of coordiates to current
				_lastx = _x;						
				_lasty = _y;						
		}
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		RepaintFigures(hdc);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}



LRESULT CALLBACK Paint::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Paint* me = reinterpret_cast<Paint*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (me) return me->RealWndProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Paint::RealWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (message){
		
		case WM_COMMAND:
			if ((HIWORD(wParam) == BN_CLICKED)){
				switch (LOWORD(wParam)){
				case ID_STRAIGHT_LINE:
					_action = ID_STRAIGHT_LINE;
					break;
				case ID_LINE:
					_action = ID_LINE;
					break;
				case ID_RECTANGLE:
					_action = ID_RECTANGLE;
					break;
				case ID_ELLIPSE:
					_action = ID_ELLIPSE;
					break;
				case ID_CLRBUTTON:
					Figures.clear();
					InvalidateRect(hwnd, NULL, FALSE);
					break;
				}
			}
			else if (HIWORD(wParam) == CBN_SELCHANGE){
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0);
				COLORREF color;

				switch (ItemIndex){
					case WHITE:
						color = RGB(255, 255, 255);
						break;
					case BLACK:
						color = RGB(0, 0, 0);
						break;
					case RED:
						color = RGB(255, 0, 0);
						break;
					case GREEN:
						color = RGB(0, 255, 0);
						break;
					case BLUE:
						color = RGB(0, 0, 255);
						break;
					}
				if (LOWORD(wParam) == ID_COLOR) {
					_color = color;
					_hPen = CreatePen(PS_SOLID, _width, _color);
				} else if (LOWORD(wParam) == ID_BGCOLOR) {
					_bgColor = color;
					_hBrush = CreateSolidBrush(_bgColor);
				}
			}
			return 0;
		case WM_HSCROLL:						// handling trackbar
			switch (LOWORD(wParam)) {
				case SB_LINELEFT:
				case TB_PAGEUP:
					_width = _width == 1 ? _width : _width - 1;
					break;
				case SB_LINERIGHT:
				case TB_PAGEDOWN:
					_width = _width == 11 ? _width : _width + 1;
					break;
				case SB_THUMBPOSITION:
					_width = HIWORD(wParam);
					break;
			}
			_hPen = CreatePen(PS_SOLID, _width, _color);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void			 Paint::RepaintFigures(HDC hdc) {
	std::vector<Figure>::iterator it;
	RECT rect;
	HWND hwnd;
	hwnd = WindowFromDC(hdc);
	for (it = Figures.begin(); it != Figures.end(); ++it) {
		SelectObject(hdc, it->hPen);
		SelectObject(hdc, it->hBrush);
		switch (it->type) {
			case ID_ELLIPSE:
				Ellipse(hdc, it->x1, it->y1, it->x2, it->y2);
				break;
			case ID_RECTANGLE:
				Rectangle(hdc, it->x1, it->y1, it->x2, it->y2);
				break;
			case ID_LINE:
			case ID_STRAIGHT_LINE:
				MoveToEx(hdc, it->x1, it->y1, NULL);
				LineTo(hdc, it->x2, it->y2);
				break;
		}
	}
}
void			 Paint::AddNewFigure() {
	Figure new_figure;

	new_figure.hBrush = _hBrush;
	new_figure.hPen = _hPen;
	new_figure.type = _action;
	new_figure.x1 = _lastx;
	new_figure.y1 = _lasty;
	new_figure.x2 = _x;
	new_figure.y2 = _y;
	Figures.push_back(new_figure);
}

void Paint::PaintEllipse(HDC hdc) {
	SelectObject(hdc, _hPen);
	SelectObject(hdc, _hBrush);

	AddNewFigure();
	Ellipse(hdc, _lastx, _lasty, _x, _y);
}
void Paint::PaintRectangle(HDC hdc) {
	SelectObject(hdc, _hPen);
	SelectObject(hdc, _hBrush);

	AddNewFigure();
	Rectangle(hdc, _lastx, _lasty, _x, _y);
}
void Paint::PaintLine(HDC hdc) {
	SelectObject(hdc, _hPen);
	SelectObject(hdc, _hBrush);

	AddNewFigure();
	MoveToEx(hdc, _lastx, _lasty, NULL);
	LineTo(hdc, _x, _y);
}