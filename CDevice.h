#pragma once

#include"Usable_Windows.h"
//TODO_Gl add include guard

#if USING_DIRECTX
#include "DirectXHeader.h"  
#endif // USING_DIRECTX


#include <cinttypes>

class CDevice
{
public:// constructors
	CDevice();
	~CDevice();
public: // functions 

	/*! used to crate a render target or equivalent
	\param BackBuffer [in] expecting it to be (ID3D11Texture2D*) or opengl equivalent
	\param RenderTraget [out] expecting it to be (ID3D11RenderTargetView*) or opengl equivalent
	*/
	bool CreateRenderTargetView(void* BackBuffer, void* RenderTraget);
	/*! used to crate a 2d texture
	\param Descriptor [in] tell us the attributes of the Texture (D3D11_TEXTURE2D_DESC) or OpenGl equivalent
	\param Texture [out] the Texture to be modified (ID3D11Texture2D*) or OpenGl equivalent*/
	bool CreateTexture2D(void * Texture, void *DescriptorTexture);
	/*! used to create a DepthSencill
	\param Texture [in] it's just a resource (ID3D11Texture2D)
	\param DiscriptorDepth [in] this will dictate how are depth-stencil will come out (D3D11_DEPTH_STENCIL_VIEW_DESC)
	\param DepthStencilView [out] this will be the resulting DepthstencilView (ID3D11DepthStencilView)
	*/
	bool CreateDepthStencilView(void* Texture, void * DiscriptorDepth, void* DepthStencilView);
	/*! Used to create the vertexShader
	\param BlobVertex [in] it's just a buffer (ID3DBlob*)
	\param VertexShader [out] The Resulting vertex-shader (ID3D11VertexShader*)
	*/
	bool CreateVertexShader(void *BlobVertex,void *VertexShader);
	/*! Used to know how to interpret the data
	\param Layout [in] To know how the data is organized (D3D11_INPUT_ELEMENT_DESC)
	\param BlobVertex [in] To obtain information about the VertexShader(ID3DBlob*)
	\param TotalElements[in] To know the amount of configuration there are (uint32_t)
	\param InputLayout [out] the resulting input layout (ID3D11InputLayout*)
	*/
	bool CreateInputLayout(void *Layout,void *BlobVertex,uint32_t TotalElements, void *InputLayout);
	/*! Used to create the pixel shader
	\param BlobPixel [in] Information about the pixel-shader.(ID3DBlob*)
	\param PixelShader [out] The resulting pixel-shader.(ID3D11PixelShader*)
	*/
	bool CreatePixelShader(void *BlobPixel,void *PixelShader);
	/*! Creates buffers 
	\param DescriptorBuffer [in] dictates how what the buffer is and it's usage (D3D11_BUFFER_DESC)
	\param Buffer [out] The Resulting buffer (ID3D11Buffer*)
	\param Data [out] used to later manipulate the information (D3D11_SUBRESOURCE_DATA)
	*/
	bool CreateBuffer(void *DescriptorBuffer, void *Buffer,void *Data);
	/*! Creates A Sampler
	\param DescriptorSampler [in] Dictates what are sampler does 
	\param Sampler [out] The Resulting sampler */
	bool CreateSamplerState(void *DescriptorSampler,void *Sampler);

	/*! This is just for testing 
	\todo when the classes "CSwapChain" and "CDeviceContext" Remove this method*/
#if defined(USING_DIRECTX)
	ID3D11Device* GetDevice();
	ID3D11Device** GetDeviceRef();
#elif
#endif

private:// variables 
	//TODO_GL replace this with equivalent
	ID3D11Device *mptr_Device = nullptr;
};
