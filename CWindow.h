#pragma once
#include "Usable_Windows.h"
#include "GLFW/glfw3.h"

/*! typedef meant for management of the  wndProc function */
using ptr_WindProc =  LRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM);

class CWindow
{
public:
	CWindow();
	~CWindow();
	/*! starts the windows */
	bool InitWindow(HINSTANCE Instance, ptr_WindProc Proc, int Width = 900, int Height = 600, int CommandShow = 10);
	/*! Destroys the window */
	void DestoryWindow();
	//! returns the height 
	int GetHeight();
	//! returns the width
	int GetWidth();
#ifdef USING_DIRECTX
	/*! returns the handler of the window in directX */
	HWND GetHandler();
	HWND& GetHandlerRef();
	/*! return a pointer to WindProc */
	ptr_WindProc GetWindProcPtr();
#else// TODO_GL


#endif // USING_DIRECTX


private:
#ifdef USING_DIRECTX
	ptr_WindProc	mptr_windProc;
	HWND m_WindowHandler;
	//! to keep track of the name and to unregister the class 
	TCHAR *m_Name;
	//! this is here to later un-register the class 
	HINSTANCE m_Instance;
	GLFWwindow* mptr_Window;
#else
#endif // USING_DIRECTX
	int m_Width;
	int m_Height;

};

