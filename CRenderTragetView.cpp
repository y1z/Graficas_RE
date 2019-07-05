#include "CRenderTragetView.h"

CRenderTragetView::CRenderTragetView()
{
#if USING_DIRECTX
	mptr_buffer = nullptr;
#endif // USING_DIRECTX

}

// destructor
CRenderTragetView::~CRenderTragetView()
{
#if USING_DIRECTX
	if (mptr_RenderTraget) { mptr_RenderTraget->Release(); }
	if (isBackBufferReleased == false && mptr_buffer != nullptr) { mptr_buffer->Release(); }
#endif // USING_DIRECTX

}

void ** CRenderTragetView::GetVoidRefRef()
{
#if USING_DIRECTX
	void ** Result = reinterpret_cast<void**>(&mptr_buffer);
	return Result;
#endif // USING_DIRECTX

	return nullptr;
}
#if USING_DIRECTX

ID3D11RenderTargetView * CRenderTragetView::GetRenderTraget()
{
	return mptr_RenderTraget;
}

ID3D11RenderTargetView ** CRenderTragetView::GetRenderTragetRef()
{
	return &mptr_RenderTraget;
}

ID3D11Texture2D * CRenderTragetView::GetBackBuffer()
{
	return mptr_buffer;
}

ID3D11Texture2D ** CRenderTragetView::GetBackBufferRef()
{
	return &mptr_buffer;
}

#elif	 USING_OPEN_GL	
uint32_t CRenderTragetView::GetBackBuffer()
{
	return this->m_BufferID;
}

uint32_t &CRenderTragetView::GetBackBufferRef()
{
	return this->m_BufferID;
}

#endif // USING_DIRECTX

void CRenderTragetView::DestroyBuffers()
{
#if USING_DIRECTX

	if (mptr_buffer)
	{
		mptr_buffer->Release();
		mptr_buffer = nullptr;
	}
	if (mptr_RenderTraget)
	{
		mptr_RenderTraget->Release();
		mptr_RenderTraget = nullptr;
	}
#endif // USING_DIRECTX


}
