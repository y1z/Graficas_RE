#include "CTexture.h"

CTexture2D::CTexture2D()
{
#if USING_DIRECTX
	memset(&m_TextureDescriptor, 0, sizeof(m_TextureDescriptor));
#endif // USING_DIRECTX

}


CTexture2D::~CTexture2D()
{
#if USING_DIRECTX
	if (mptr_Texture) { mptr_Texture->Release(); }
#endif // USING_DIRECTX

}
 
void CTexture2D::InitTexture2D(uint32_t Width, uint32_t Hight, int32_t Format, int32_t BindFlag)
{
#if(USING_DIRECTX)
	/*should be the same size as the window */
	m_TextureDescriptor.Height = Hight;
	m_TextureDescriptor.Width = Width;
	// default values 
	m_TextureDescriptor.Usage = D3D11_USAGE_DEFAULT;
	m_TextureDescriptor.SampleDesc.Count = 1;
	m_TextureDescriptor.SampleDesc.Quality = 0;
	/*casting to the native format for directX  */
	m_TextureDescriptor.BindFlags = static_cast<D3D11_BIND_FLAG>(BindFlag);
	/*casting to the native format for directX  */
	m_TextureDescriptor.Format = static_cast<DXGI_FORMAT>(Format);
	m_TextureDescriptor.MipLevels = 1;
	m_TextureDescriptor.ArraySize = 1;
	m_TextureDescriptor.CPUAccessFlags = 0;
	m_TextureDescriptor.MiscFlags = 0;

#elif USING_OPEN_GL


#endif

}

void CTexture2D::MakeNull()
{
#if USING_DIRECTX
	mptr_Texture = nullptr;
#endif // USING_DIRECTX
}
#if USING_DIRECTX

ID3D11Texture2D * CTexture2D::GetTexture()
{
	return mptr_Texture;
}

ID3D11Texture2D ** CTexture2D::GetTextureRef()
{
	return &mptr_Texture;
}

D3D11_TEXTURE2D_DESC CTexture2D::GetDescriptor()
{
	return m_TextureDescriptor;
}
#endif // USING_DIRECTX

