#pragma once
#if USING_OPEN_GL
#undef USING_DIRECTX
#endif


#if USING_DIRECTX
#include "DirectXHeader.h" 
#elif USING_OPEN_GL
#include "OpenglHeader.h"
#endif // USING_DIRECTX
#include	"Usable_Windows.h"


#include <cinttypes>
class CTexture2D
{
public:// constructors 
	CTexture2D();
	~CTexture2D();
public:// functions
	/*! this function prepares the texture for creation*/
	void InitTexture2D(uint32_t Width, uint32_t Hight,int32_t Format,int32_t BindFlag);

	void MakeNull();


public:
#if(USING_DIRECTX)
	ID3D11Texture2D *GetTexture();
	ID3D11Texture2D **GetTextureRef();

	D3D11_TEXTURE2D_DESC GetDescriptor();

#else//TODO_GL

	uint32_t GetTexture();
	uint32_t &GetTextureRef();
	uint32_t GetWidth();
	uint32_t GetHeight();


#endif
private:// variables
#if(USING_DIRECTX)
	ID3D11Texture2D *mptr_Texture = nullptr;
	D3D11_TEXTURE2D_DESC m_TextureDescriptor;
#elif USING_OPEN_GL// TODO_GL 
	uint32_t m_TextureID = 0;
#endif
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
};

