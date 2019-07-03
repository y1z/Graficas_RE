#include "CRenderTragetView.h"

CRenderTragetView::CRenderTragetView()
{
	mptr_buffer = nullptr;
}

// destructor
CRenderTragetView::~CRenderTragetView()
{
	if (mptr_RenderTraget) { mptr_RenderTraget->Release(); }
	if (isBackBufferReleased == false && mptr_buffer != nullptr) { mptr_buffer->Release(); }
}

void ** CRenderTragetView::GetVoidRefRef()
{
	void ** Result = reinterpret_cast<void**>(&mptr_buffer);

	return Result;
}

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

void CRenderTragetView::DestroyBuffers()
{

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


}
