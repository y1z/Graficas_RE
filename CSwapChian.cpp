#include "CSwapChian.h"
#include "CDevice.h"
#include "CBuffer.h"
#include "CRenderTragetView.h"

CSwapChian::CSwapChian()
{}


CSwapChian::~CSwapChian()
{
	if (mptr_SwapChian != nullptr) { mptr_SwapChian->Release(); }
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
#elif
#endif

	return false;
}

bool CSwapChian::Present(int32_t Syc, int32_t PresentOpction)
{
#if defined(USING_DIRECTX)
	HRESULT hr = S_OK;
	hr = mptr_SwapChian->Present(Syc, PresentOpction);

	if (!CheckForError(hr))
	{
		return true;
	}
	return false;

#elif
#endif

	return false;
}

void CSwapChian::ResizeBuffer(int width, int height, HWND g_hWnd)
{
	mptr_SwapChian->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
}

void CSwapChian::ResizeTarget(int width, int height)
{
	DXGI_MODE_DESC Description;
	Description.Format = DXGI_FORMAT_UNKNOWN;
	Description.Height = height;
	Description.Width = width;
	Description.RefreshRate.Numerator = 60;
	Description.RefreshRate.Denominator = 1;
	Description.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	Description.Scaling = DXGI_MODE_SCALING_STRETCHED;

	DXGI_SWAP_CHAIN_DESC temp;

	mptr_SwapChian->GetDesc(&temp);

}

IDXGISwapChain ** CSwapChian::GetSwapChianRef()
{
	return &mptr_SwapChian;
}
