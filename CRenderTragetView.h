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
	ID3D11Texture2D*GetBackBuffer();
	ID3D11Texture2D** GetBackBufferRef();
#elif USING_OPEN_GL
	uint32_t GetBackBuffer();
	uint32_t &GetBackBufferRef();
#endif
	void DestroyBuffers();

	void ** GetVoidRefRef();
private:// variables 
	bool isBackBufferReleased = false;
#if defined (USING_DIRECTX)
	ID3D11RenderTargetView *mptr_RenderTraget = nullptr;
	ID3D11Texture2D * mptr_buffer = nullptr;
#elif USING_OPEN_GL
	uint32_t m_BufferID;
	uint32_t m_RenderTragetID;
#endif
};
