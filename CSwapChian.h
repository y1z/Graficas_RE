#pragma once
#include "Usable_Windows.h"
#include "GraphicsLibsHeader.h"
#include <cinttypes>
#include "Utility/ErrorHandlingGrafics.h"
class CBuffer;
class CRenderTragetView;

class CSwapChian
{
public:
	CSwapChian();
	~CSwapChian();
public:// functions

	bool GetBuffer(int32_t BufferIndex, CRenderTragetView &RenderTraget);
	bool Present(int32_t Syc = 0, unsigned int PresentOpction = 0);

	void ResizeBuffer(int width, int height, HWND hWnd);
	void ResizeTarget(int width, int height);
	/*! releases the current swap chain and make it nullptr*/
	void DestroySelf();
#if USING_DIRECTX
	/*! This is just for creating the SwapChian*/
	IDXGISwapChain** GetSwapChianRef();
private:// variable
	IDXGISwapChain* mptr_SwapChian = nullptr;
#elif USING_OPEN_GL
	//! used in openGL need to be able to manipulate the render-target back-buffer 
	unsigned int *m_BackBufferID;
	//! this is used to present an image an screen 
	GLFWwindow *ptr_Window;
#endif // USING_DIRECTX
};
