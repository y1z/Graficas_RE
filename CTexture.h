#pragma once
#include	"Usable_Windows.h"
#include "DirectXHeader.h"
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

#if(USING_DIRECTX)
	ID3D11Texture2D *GetTexture();
	ID3D11Texture2D **GetTextureRef();

	D3D11_TEXTURE2D_DESC GetDescriptor();

#else//TODO_GL
#endif
private:// variables
#if(USING_DIRECTX)
	ID3D11Texture2D *mptr_Texture = nullptr;
	D3D11_TEXTURE2D_DESC m_TextureDescriptor;
#else// TODO_GL 
#endif
};

