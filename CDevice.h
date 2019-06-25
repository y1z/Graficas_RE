#pragma once
// all classes created by CDevice
class CBuffer;
class CSampler;
class CSwapChian;
class CDeviaceContext;
class CWindow;
class CRenderTragetView;
class CTexture2D;
class CDepthStencilView;
class CVertexShader;
class CInputLayout;
#include"Usable_Windows.h"
//TODO_Gl add include guard
#include "CWindow.h"

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
	/*!
	\param [out] SwapChian used to also init SwapChian 
	\param [out] DeviaceContext used to init DeviaceContext
	\param [in] used to know which Window to associate with Device,SwapChian and DeviceContext to the Window*/
	bool InitDevice(CSwapChian &SwapChian, CDeviaceContext &DeviaceContext,CWindow &Window);

	/*! used to crate a render target or equivalent
	\param RenderTragetView [out] This class Contains both the back buffer and render-target necessary for making rendertargetView.
	*/
	bool CreateRenderTargetView(CRenderTragetView &RenderTragetView);
	/*! used to crate a 2d texture
	\param Texture [out] the Texture to be modified */
	bool CreateTexture2D(CTexture2D &Texture);
	/*! used to create a DepthSencill
	\param DepthStencilView [out] this will be the resulting DepthstencilView 
	*/
	bool CreateDepthStencilView(CDepthStencilView &DepthStencilView);
	/*! Used to create the vertexShader
	\param BlobVertex [in] it's just a buffer (ID3DBlob*)
	\param VertexShader [out] The Resulting vertex-shader (ID3D11VertexShader*)
	*/
	bool CreateVertexShader(CVertexShader &VertexShader);
	/*! Used to know how to interpret the data
	\param Layout [in] To know how the data is organized (D3D11_INPUT_ELEMENT_DESC)
	\param BlobVertex [in] To obtain information about the VertexShader(ID3DBlob*)
	\param TotalElements[in] To know the amount of configuration there are (uint32_t)
	\param InputLayout [out] the resulting input layout (ID3D11InputLayout*)
	*/
	bool CreateInputLayout(CInputLayout &Layout, CVertexShader &VertexShader);
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
