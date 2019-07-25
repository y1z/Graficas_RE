#include "CWindow.h"
#include  "Resource.h"

#ifdef UNICODE
static constexpr TCHAR *DefulatName = L"window";
#else
static constexpr TCHAR *DefulatName = "window";
#endif // UNICODE


CWindow::CWindow()
{}


CWindow::~CWindow()
{}

bool CWindow::InitWindow(HINSTANCE Instance, ptr_WindProc Proc, int Width, int Height, int CommandShow)
{
	mptr_windProc = Proc;
	m_Height = Height;
	m_Width = Width;
	m_Name = DefulatName;

#ifdef USING_DIRECTX
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
	wcex.lpszClassName = DefulatName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR) IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	RECT rc = { 0, 0, m_Width, m_Height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_WindowHandler = CreateWindow(DefulatName, L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, Instance,
		NULL);
	if (!m_WindowHandler)
		return false;

	ShowWindow(m_WindowHandler, CommandShow);
#elif USING_OPEN_GL

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	mptr_Window = glfwCreateWindow(m_Width, m_Height, "OpenGL Window", nullptr, nullptr);

	glfwMakeContextCurrent(mptr_Window);

	if (!mptr_Window)
	{
		glfwTerminate();
		MessageBox(nullptr, L"ERROR!", L"Error With glfw Window ", 0);
		return false;
	}

#endif // USING_DIRECTX

	return true;
}

void CWindow::DestoryWindow()
{
#ifdef USING_DIRECTX
	UnregisterClass(m_Name, m_Instance);

#else
	glfwDestroyWindow(mptr_Window);
#endif // USING_DIRECTX

}

int CWindow::GetHeight()
{
	return m_Height;
}

int CWindow::GetWidth()
{
	return m_Width;
}
#if USING_DIRECTX

HWND CWindow::GetHandler()
{
	return m_WindowHandler;
}

HWND & CWindow::GetHandlerRef()
{
	return m_WindowHandler;
}
#elif USING_OPEN_GL
GLFWwindow *  CWindow::GetHandler()
{
	return	mptr_Window;
}

#endif // USING_DIRECTX

ptr_WindProc CWindow::GetWindProcPtr()
{
	return mptr_windProc;
}

