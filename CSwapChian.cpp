#include "CSwapChian.h"



CSwapChian::CSwapChian()
{
}


CSwapChian::~CSwapChian()
{
	if (mptr_SwapChian != nullptr) { mptr_SwapChian->Release(); }
}

bool CSwapChian::GetBuffer(int32_t BufferIndex, void * Buffer)
{
#if defined(USING_DIRECTX)
	
	HRESULT hr = S_OK;

	hr = mptr_SwapChian->GetBuffer(BufferIndex, __uuidof(ID3D11Texture2D), static_cast<void**>(Buffer));

	if (!CheckForError(hr)) {
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

	if (!CheckForError(hr)) {
		return true;
	}
	return false;

#elif
#endif

	return false;
}

IDXGISwapChain ** CSwapChian::GetSwapChianRef()
{
	return &mptr_SwapChian;
}
