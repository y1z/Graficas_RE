#include "CDevice.h"

#include "Utility/ErrorHandlingGrafics.h"
/* often used preprocessor statements
#if defined(USING_DIRECTX)
#elif
#endif
*/


CDevice::CDevice()
{

}

//Descriptor

CDevice::~CDevice()
{
	if (mptr_Device != nullptr) { mptr_Device->Release(); }
}

bool CDevice::CreateRenderTargetView(void * BackBuffer, void * RenderTraget)
{
#if defined(USING_DIRECTX)

	HRESULT hr;

	hr = mptr_Device->CreateRenderTargetView(reinterpret_cast<ID3D11Texture2D*>(BackBuffer),
		nullptr,
		reinterpret_cast<ID3D11RenderTargetView**> (RenderTraget));
	if (!CheckForError(hr))
	{
		return true;
	}
	else {
		return false;
	}


#elif//! TODO add opengl Functionality


#endif

	return false;
}

bool CDevice::CreateTexture2D(void * Texture, void * Descriptor)
{
#if defined(USING_DIRECTX)

	HRESULT hr = S_OK;

	hr = mptr_Device->CreateTexture2D(static_cast<D3D11_TEXTURE2D_DESC*>(Descriptor),
		nullptr,
		static_cast<ID3D11Texture2D**>(Texture));
	// For knowing if something went wrong 
	if (!CheckForError(hr))
	{
		return true;
	}

	return false;

#elif//! TODO add opengl Functionality


#endif

	return false;
}

bool CDevice::CreateDepthStencilView(void * Texture, void * DiscriptorDepth, void * DepthStencilView)
{
#if defined(USING_DIRECTX)

	HRESULT hr = S_OK;

	hr = mptr_Device->CreateDepthStencilView(static_cast<ID3D11Texture2D*>(Texture),
		static_cast<D3D11_DEPTH_STENCIL_VIEW_DESC *>(DiscriptorDepth),
		static_cast <ID3D11DepthStencilView * *>(DepthStencilView));

	// For knowing if something went wrong 
	if (!CheckForError(hr))
	{
		return true;
	}

	return false;

#elif//! TODO add opengl Functionality


#endif
	return false;
}

bool CDevice::CreateVertexShader(void * BlobVertex, void * VertexShader)
{
#if defined(USING_DIRECTX)

	HRESULT hr = S_OK;

	ID3DBlob * ptr_temp = static_cast<ID3DBlob*>(BlobVertex);

	hr = mptr_Device->CreateVertexShader(ptr_temp->GetBufferPointer(),
		ptr_temp->GetBufferSize(),
		nullptr,
		static_cast<ID3D11VertexShader**>(VertexShader));

	if (!CheckForError(hr)) {
		return true;
	}

	return false;
#elif//TODO_GL

#endif

	return false;
}

bool CDevice::CreateInputLayout(void * Layout, void * BlobVertex, uint32_t TotalElements, void * InputLayout)
{
#if defined(USING_DIRECTX)

	ID3DBlob * ptr_temp = static_cast<ID3DBlob*>(BlobVertex);

	HRESULT hr = S_OK;
	hr = mptr_Device->CreateInputLayout(static_cast<D3D11_INPUT_ELEMENT_DESC*>(Layout),
		TotalElements,
		ptr_temp->GetBufferPointer(),
		ptr_temp->GetBufferSize(),
		static_cast<ID3D11InputLayout**>(InputLayout));

	if (!CheckForError(hr))
	{
		return true;
	}

	return false;
#elif
#endif
	return false;
}

bool CDevice::CreatePixelShader(void * BlobPixel, void * PixelShader)
{
#if defined(USING_DIRECTX)
	ID3DBlob *ptr_temp = static_cast<ID3DBlob*> (BlobPixel);
	HRESULT hr = S_OK;

	mptr_Device->CreatePixelShader(ptr_temp->GetBufferPointer(),
		ptr_temp->GetBufferSize(),
		nullptr,
		static_cast<ID3D11PixelShader**>(PixelShader));

	if (!CheckForError(hr)) {
		return true;
	}

	return false;

#elif
#endif
	return false;
}

bool CDevice::CreateBuffer(void * Descriptor, void * Buffer, void * Data = nullptr)
{
#if defined(USING_DIRECTX)

	HRESULT hr = S_OK;
	if (Data != nullptr)
	{
		hr = mptr_Device->CreateBuffer(static_cast<D3D11_BUFFER_DESC*>(Descriptor),
			static_cast<D3D11_SUBRESOURCE_DATA*>(Data),
			static_cast<ID3D11Buffer**>(Buffer));
	}
	else {
		hr = mptr_Device->CreateBuffer(static_cast<D3D11_BUFFER_DESC*>(Descriptor),
			nullptr,
			static_cast<ID3D11Buffer**>(Buffer));
	}


	if (!CheckForError(hr))
	{
		return true;
	}

	return false;
#elif
#endif

	return false;
	}

bool CDevice::CreateSamplerState(void * DescriptorSampler, void * Sampler)
{
#if defined(USING_DIRECTX)
	HRESULT hr = S_OK;

	hr = mptr_Device->CreateSamplerState(static_cast<D3D11_SAMPLER_DESC*>(DescriptorSampler),
		static_cast<ID3D11SamplerState**>(Sampler));

	if (!CheckForError(hr))
	{
		return true;
	}

	return false;

#elif
#endif

	return false;
}

#if defined(USING_DIRECTX)
ID3D11Device * CDevice::GetDeviceTemp()
{
	return mptr_Device;
}

ID3D11Device ** CDevice::GetDeviceRef()
{
	return &mptr_Device;
}
#elif
#endif 
