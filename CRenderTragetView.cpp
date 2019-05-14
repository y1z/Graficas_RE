#include "CRenderTragetView.h"

CRenderTragetView::CRenderTragetView()
{
	mptr_buffer = nullptr;
}

// destructor
CRenderTragetView::~CRenderTragetView()
{
	if (mptr_RenderTraget) { mptr_RenderTraget->Release(); }
	if (mptr_DepthStencilView) { mptr_DepthStencilView->Release(); }
	if (isBackBufferReleased == false && mptr_buffer != nullptr) { mptr_buffer->Release(); }
}

ID3D11RenderTargetView * CRenderTragetView::GetRenderTraget()
{
	return mptr_RenderTraget;
}

ID3D11RenderTargetView ** CRenderTragetView::GetRenderTragetRef()
{
	return &mptr_RenderTraget;
}

ID3D11DepthStencilView * CRenderTragetView::GetDepthStencilView()
{
	return mptr_DepthStencilView;
}

ID3D11DepthStencilView ** CRenderTragetView::GetDepthStencilViewRef()
{
	return &mptr_DepthStencilView;
}

ID3D11Texture2D * CRenderTragetView::GetBackBuffer()
{
	return mptr_buffer;
}

ID3D11Texture2D ** CRenderTragetView::GetBackBufferRef()
{
	return &mptr_buffer;
}

void CRenderTragetView::ReleaseBackBuffer()
{
	mptr_buffer->Release();
	// just in case
	isBackBufferReleased = true;
}
