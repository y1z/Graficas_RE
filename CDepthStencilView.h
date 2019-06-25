#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include "CTexture.h"
/**
@brief here is an abstraction of the DepthStencilview class from DirectX
*/

//! This is an intermediate of DirectX Depth-Stencil descriptor and my own types 
struct SDepthStencilView
{
	int Format; // the same as D3D11_DEPTH_STENCIL_VIEW_DESC -> Format
	int AccessFlags; // the same as D3D11_DEPTH_STENCIL_VIEW_DESC -> ViewDimension
	//! indicates the texture being used 
	int TextureType;// 1 represents a 1d texture 2 represents 2d and so on .
	bool isTextureArray = false;//< to know if the texture is an array 
		//! specify if the texture is read-only or not, 0 is equal to "yes you can also write" 
	uint32_t ReadingType;// the same as D3D11_DEPTH_STENCIL_VIEW_DESC -> flags
};


class CDepthStencilView
{
public:// constructor 
	CDepthStencilView();
	~CDepthStencilView();
public:// functions
	/*! To get the necessary information needed for the Depth-Stencil-view*/
	void InitDepthStencil2D(uint32_t Height, uint32_t Width, int Format);
	//! for functions that require pointers 
	CTexture2D * GetTexture2D();
	//! for functions that require two pointers 
	CTexture2D & GetTexture2DRef();

#ifdef USING_DIRECTX
	//! for functions that require two pointers 
	ID3D11DepthStencilView* GetDepthStencilView();
	//! for functions that require two pointers 
	ID3D11DepthStencilView** GetDepthStencilViewRef();
	//! for parameters that need the texture description
	D3D11_TEXTURE2D_DESC GetTextureDescriptor();
	//! take my struct and converts it to DirectX native struct.
	D3D11_DEPTH_STENCIL_VIEW_DESC ConvertDepthStecilToDx2D();
#endif // USING_DIRECTX

	void DestoryBuffer();

private://variables
#if USING_DIRECTX
			/*! for interfacing with directX */
	ID3D11DepthStencilView* mptr_DepthStencilView = nullptr;
#endif // USING_DIRECTX

	/*! the texture to project on to */
	CTexture2D *mptr_DepthStencil = nullptr;
	/*! an Intermediate class for my classes and directX */
	SDepthStencilView m_Descriptor;
};

