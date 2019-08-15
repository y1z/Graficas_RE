#include "CSwapChian.h"
#include "CDevice.h"
#include "CBuffer.h"
#include "CRenderTragetView.h"
#include <cassert>

CSwapChian::CSwapChian()
{
#if USING_DIRECTX
	mptr_SwapChian = nullptr;
#elif USING_OPEN_GL
	m_BackBufferID = nullptr;
	ptr_Window = nullptr;
#endif
}


CSwapChian::~CSwapChian()
{
#if USING_DIRECTX
	if (mptr_SwapChian != nullptr) { mptr_SwapChian->Release(); }
#elif	USING_OPEN_GL
	m_BackBufferID = nullptr;
#endif // USING_DIRECTX

}



bool CSwapChian::GetBuffer(int32_t BufferIndex, CRenderTragetView &RenderTraget)
{
#if defined(USING_DIRECTX)

	HRESULT hr = S_OK;
	/// later make safe 
	hr = mptr_SwapChian->GetBuffer(BufferIndex, __uuidof(ID3D11Texture2D), RenderTraget.GetVoidRefRef());
	if (!CheckForError(hr))
	{
		return true;
	}

	return false;
#elif	USING_OPEN_GL

	this->m_BackBufferID = RenderTraget.GetBackBufferRef();

	return true;
#endif

	return false;
}

bool CSwapChian::Present(int32_t Syc, unsigned int PresentOpction)
{
#if defined(USING_DIRECTX)
	HRESULT hr = S_OK;

	hr = mptr_SwapChian->Present(Syc, PresentOpction);

	if (!CheckForError(hr))
	{
		return true;
	}
	return false;

#elif USING_OPEN_GL
	//glEnableVertexAttribArray(0);

	glfwPollEvents();

	glfwSwapBuffers(this->ptr_Window);

	if (! GlCheckForError())
	{
		return true;
	}


#endif

	return false;
}

void CSwapChian::ResizeBuffer(int width, int height, HWND hWnd)
{
#if USING_DIRECTX

	HRESULT hr =	mptr_SwapChian->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	assert( CheckForError(hr) == false && "Erro with Resizing buffers ");
#endif // USING_DIRECTX

}

void CSwapChian::ResizeTarget(int width, int height)
{
#if USING_DIRECTX
	DXGI_MODE_DESC Description;
	Description.Format = DXGI_FORMAT_UNKNOWN;
	Description.Height = height;
	Description.Width = width;
	Description.RefreshRate.Numerator = 60;
	Description.RefreshRate.Denominator = 1;
	Description.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	Description.Scaling = DXGI_MODE_SCALING_STRETCHED;

	mptr_SwapChian->ResizeTarget(&Description);

#endif // USING_DIRECTX


}

void CSwapChian::DestroySelf()
{
#ifdef USING_DIRECTX
	mptr_SwapChian->Release();
	mptr_SwapChian = nullptr;

#endif // USING_DIRECTX

}

#if USING_DIRECTX
IDXGISwapChain ** CSwapChian::GetSwapChianRef()
{
	return &mptr_SwapChian;
}

#endif // USING_DIRECTX
