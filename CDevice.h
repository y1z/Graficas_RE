#pragma once
// all classes created by CDevice
class CBuffer;
class CSampler;
class CSwapChian;
class CDeviceContext;
class CWindow;
class CRenderTragetView;
class CTexture2D;
class CDepthStencilView;
class CVertexShader;
class CInputLayout;
class CPixelShader;
class CVertexShader;
class CSampler;

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
	bool InitDevice(CSwapChian &SwapChian, CDeviceContext &DeviaceContext,CWindow &Window);

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
	\param Layout [out] will tell me how the data is organized 
	\param VertexShader [in] this is where we get how the data is organized  
	*/
	bool CreateInputLayout(CInputLayout &Layout, CVertexShader &VertexShader);
	/*! Used to create the pixel shader
	\param PixelShader [out] The resulting pixel-shader.
	*/
	bool CreatePixelShader(CPixelShader &PixelShader);
	/*! Creates buffers 
	\param Buffer [out] The Resulting buffer
	*/
	bool CreateBuffer(CBuffer &Buffer);
	/*! Creates A Sampler
	\param DescriptorSampler [in] Dictates what are sampler does 
	\param Sampler [out] The Resulting sampler */
	bool CreateSamplerState(CSampler &Sampler);

	/*! This is just for testing 
	\todo when the classes "CSwapChain" and "CDeviceContext" Remove this method*/
#if defined(USING_DIRECTX)
	ID3D11Device* GetDevice();
	ID3D11Device** GetDeviceRef();
#else
#endif

private:// variables 
	//TODO_GL replace this with equivalent
#if USING_DIRECTX
	ID3D11Device *mptr_Device = nullptr;
#endif // USING_DIRECTX

};
