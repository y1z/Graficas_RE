#pragma once
#include "Usable_Windows.h"
using ptr_WindProc = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);

class CWindow
{
public:
	CWindow();
	~CWindow();

	bool InitWindow(HINSTANCE Instance,ptr_WindProc Proc, int Width = 600, int Height = 800,int CommandShow = 10);

	
	int GetHeight();
	int GetWidth();
	HWND GetHandler();
	HWND& GetHandlerRef();
	ptr_WindProc GetWindProcPtr();
	
private:
	ptr_WindProc	mptr_windProc;
	HWND m_WindowHandler;
	int m_Width;
	int m_Height;

};

