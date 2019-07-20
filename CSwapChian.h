#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include "CDevice.h"
#include <cinttypes>

#include "Utility/ErrorHandlingGrafics.h"
class CBuffer;

class CSwapChian
{
public:
	CSwapChian();
	~CSwapChian();
public:// functions

	bool GetBuffer(int32_t BufferIndex, CRenderTragetView &RenderTraget);
	bool Present(int32_t Syc = 0 , unsigned int PresentOpction = 0);

	void ResizeBuffer(int width, int height, HWND g_hWnd);
	void ResizeTarget(int width, int height);
#if USING_DIRECTX
	/*! This is just for creating the SwapChian*/
	IDXGISwapChain** GetSwapChianRef();
public:// variable
	IDXGISwapChain* mptr_SwapChian = nullptr;
#elif USING_OPEN_GL
	//! used in openGL need to be able to manipulate the render-target back-buffer 
	unsigned int *m_BackBufferID;
	//! this is used to present an image an screen 
	GLFWwindow *ptr_Window;
#endif // USING_DIRECTX



};

