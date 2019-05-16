#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include <cinttypes>
#include	"CTexture.h"


class CRenderTragetView
{
public:// constructor 
	CRenderTragetView();
	~CRenderTragetView();
public:// functions
#if defined (USING_DIRECTX)
	ID3D11RenderTargetView * GetRenderTraget();
/*! For Arguments that require more that 1 pointer*/
	ID3D11RenderTargetView ** GetRenderTragetRef();
	ID3D11DepthStencilView * GetDepthStencilView();
	ID3D11DepthStencilView ** GetDepthStencilViewRef();
	ID3D11Texture2D *GetBackBuffer();
	ID3D11Texture2D ** GetBackBufferRef();

	void ReleaseBackBuffer();
#elif
#endif

private:// variables 
	bool isBackBufferReleased = false;
#if defined (USING_DIRECTX)
	ID3D11RenderTargetView *mptr_RenderTraget = nullptr;
	ID3D11DepthStencilView *mptr_DepthStencilView = nullptr;
	ID3D11Texture2D * mptr_buffer = nullptr;
#elif
#endif
};
