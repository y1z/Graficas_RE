#pragma once
#include  "Usable_Windows.h"
#include  "GraphicsLIbsHeader.h"

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
	
	ptr_WindProc GetWindProcPtr();

	bool CheckIsActive() const ;
	/*! returns the handler of the window in directX */
#ifdef USING_DIRECTX

	HWND GetHandler();
	HWND& GetHandlerRef();
	/*! return a pointer to WindProc */
#else// TODO_GL
	GLFWwindow *GetHandler();

#endif // USING_DIRECTX


private:
	ptr_WindProc	mptr_windProc;
	TCHAR *m_Name;
#ifdef USING_DIRECTX
	HWND m_WindowHandler;
	//! to keep track of the name and to unregister the class 
	//! this is here to later un-register the class 
	HINSTANCE m_Instance;
#elif USING_OPEN_GL
	GLFWwindow* mptr_Window;
#endif // USING_DIRECTX
	int m_Width;
	int m_Height;
	bool IsActive = false;

};

