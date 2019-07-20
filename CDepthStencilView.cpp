#include "CDepthStencilView.h"
#include "Utility/ErrorHandlingGrafics.h"
#include <cassert>
CDepthStencilView::CDepthStencilView()
{
	mptr_DepthStencil = new CTexture2D();
}


CDepthStencilView::~CDepthStencilView()
{
	if (mptr_DepthStencil != nullptr) { delete mptr_DepthStencil; }
#if USING_DIRECTX
	if (mptr_DepthStencilView != nullptr) { mptr_DepthStencilView->Release(); }
#elif USING_OPEN_GL
	if (m_DepthBufferID != 0) { glDeleteFramebuffers(1, &m_DepthBufferID); }
#endif // USING_DIRECTX

}

void CDepthStencilView::InitDepthStencil2D(uint32_t Height, uint32_t Width, int Format)
{
#ifdef USING_DIRECTX
	mptr_DepthStencil->InitTexture2D(Width, Height, Format, static_cast<int>(D3D11_BIND_DEPTH_STENCIL));
	m_Descriptor.Format = mptr_DepthStencil->GetDescriptor().Format;
	m_Descriptor.AccessFlags = (int) D3D11_DSV_DIMENSION_TEXTURE2D;
	m_Descriptor.TextureType = 2;
	m_Descriptor.ReadingType = 0; //means that you can write to this texture
#elif USING_OPEN_GL
	GlRemoveAllErrors();
	glEnable(Format);
	if (!GlCheckForError())
	{
		return;
	}
	else
	{
		assert(true == false);
	}

#endif // USING_DIRECTX
}


#ifdef USING_DIRECTX
ID3D11DepthStencilView * CDepthStencilView::GetDepthStencilView()
{
	return mptr_DepthStencilView;
}
#endif // USING_DIRECTX
#ifdef USING_DIRECTX
ID3D11DepthStencilView ** CDepthStencilView::GetDepthStencilViewRef()
{
	return &mptr_DepthStencilView;
}
#endif // USING_DIRECTX
CTexture2D * CDepthStencilView::GetTexture2D()
{
	return mptr_DepthStencil;
}

CTexture2D & CDepthStencilView::GetTexture2DRef()
{
	return  *mptr_DepthStencil;
}
#ifdef USING_DIRECTX
D3D11_TEXTURE2D_DESC CDepthStencilView::GetTextureDescriptor()
{
	return mptr_DepthStencil->GetDescriptor();
}
#endif // USING_DIRECTX

#ifdef USING_DIRECTX
D3D11_DEPTH_STENCIL_VIEW_DESC CDepthStencilView::ConvertDepthStecilToDx2D()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC Result;
	Result.Format = static_cast<DXGI_FORMAT>(m_Descriptor.Format);
	Result.ViewDimension = static_cast<D3D11_DSV_DIMENSION>(m_Descriptor.AccessFlags);
	Result.Flags = m_Descriptor.TextureType;
	Result.Texture2D.MipSlice = 0;

	return Result;
}
#elif USING_OPEN_GL


uint32_t CDepthStencilView::GetDepthBuffer()
{
	return m_DepthBufferID;
}

uint32_t & CDepthStencilView::GetDepthBufferRef()
{
	return m_DepthBufferID;
}


#endif // USING_DIRECTX




void CDepthStencilView::DestoryBuffer()
{
	if (mptr_DepthStencil)
	{
	#if USING_DIRECTX
		mptr_DepthStencil->GetTexture()->Release();
	#endif // USING_DIRECTX

		mptr_DepthStencil->MakeNull();

	}

}