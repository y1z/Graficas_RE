#include "CWindow.h"
#include "Resource.h"

CWindow::CWindow()
{}


CWindow::~CWindow()
{}

bool CWindow::InitWindow(HINSTANCE Instance, ptr_WindProc Proc,int Width,int Height, int CommandShow)
{
	// assign winpro to variable 
	mptr_windProc = Proc;
	m_Height = Height;
	m_Width = Width;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Proc;// function pointer to winproc 
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = LoadIcon(Instance, (LPCTSTR) IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR) IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_WindowHandler = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, Instance,
		NULL);
	if (!m_WindowHandler)
		return false;

	ShowWindow(m_WindowHandler, CommandShow);

	return true;
}

int CWindow::GetHeight()
{
	return m_Height;
}

int CWindow::GetWidth()
{
	return m_Width;
}

HWND CWindow::GetHandler()
{
	return m_WindowHandler;
}

HWND & CWindow::GetHandlerRef()
{
	return m_WindowHandler;
}

ptr_WindProc CWindow::GetWindProcPtr()
{
	return mptr_windProc;
}

