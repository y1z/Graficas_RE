#include "CDevice.h"
#include "CSwapChian.h"
#include "CDeviaceContext.h"
#include "CRenderTragetView.h"
#include "CDepthStencilView.h"
#include "CVertexShader.h"
#include "CInputLayout.h"

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

bool CDevice::InitDevice(CSwapChian &SwapChian, CDeviaceContext &DeviaceContext, CWindow &Window)
{

#if USING_DIRECTX
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(Window.GetHandler(), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE SelectedDriver = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL SelectedFeatureLevel = D3D_FEATURE_LEVEL_10_0;


	D3D_DRIVER_TYPE driverTypes[] =
	{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = Window.GetHandlerRef();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		SelectedDriver = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, SelectedDriver, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, SwapChian.GetSwapChianRef(), this->GetDeviceRef(), &SelectedFeatureLevel, DeviaceContext.GetDeviceContextRef());
		if (SUCCEEDED(hr))
		{
			return true;
		}
	}
	if (FAILED(hr))
		return false;
#endif // USING_DIRECTX

	return false;
}

bool CDevice::CreateRenderTargetView(CRenderTragetView &RenderTragetView)
{
#if defined(USING_DIRECTX)

	HRESULT hr;

	hr = mptr_Device->CreateRenderTargetView(reinterpret_cast<ID3D11Texture2D*>(RenderTragetView.GetBackBuffer()),
		nullptr,
		reinterpret_cast<ID3D11RenderTargetView**> (RenderTragetView.GetRenderTragetRef()));

	if (!CheckForError(hr))
	{
		return true;
	}
	else
	{
		return false;
	}


#elif//! TODO add opengl Functionality


#endif

	return false;
}

bool CDevice::CreateTexture2D(CTexture2D &Texture)
{
#if defined(USING_DIRECTX)

	HRESULT hr = S_OK;

	hr = mptr_Device->CreateTexture2D(static_cast<D3D11_TEXTURE2D_DESC*>(&Texture.GetDescriptor()),
		nullptr,
		static_cast<ID3D11Texture2D**>(Texture.GetTextureRef()));
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

bool CDevice::CreateDepthStencilView(CDepthStencilView &DepthStencilView)
{
#if defined(USING_DIRECTX)

	HRESULT hr = S_OK;

	hr = mptr_Device->CreateDepthStencilView(static_cast<ID3D11Texture2D*>(DepthStencilView.GetTexture2D()->GetTexture()),
		static_cast<D3D11_DEPTH_STENCIL_VIEW_DESC *>(&DepthStencilView.ConvertDepthStecilToDx2D()),
		static_cast <ID3D11DepthStencilView **>(DepthStencilView.GetDepthStencilViewRef()));

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

bool CDevice::CreateVertexShader(CVertexShader &VertexShader)
{
#if defined(USING_DIRECTX)

	HRESULT hr = S_OK;

	ID3DBlob * ptr_temp = static_cast<ID3DBlob*>(VertexShader.GetVertexShaderData());

	hr = mptr_Device->CreateVertexShader(ptr_temp->GetBufferPointer(),
		ptr_temp->GetBufferSize(),
		nullptr,
		static_cast<ID3D11VertexShader**>(VertexShader.GetVertexShaderRef()));

	if (!CheckForError(hr))
	{
		return true;
	}

	return false;
#elif//TODO_GL

#endif

	return false;
}

bool CDevice::CreateInputLayout(CInputLayout &Layout, CVertexShader &VertexShader)
{
#if defined(USING_DIRECTX)

	ID3DBlob * ptr_temp = static_cast<ID3DBlob*>(VertexShader.GetVertexShaderData());
	auto InputLayoutDesc = Layout.ConvertInputLayoutToDx();
	HRESULT hr = S_OK;


	hr = mptr_Device->CreateInputLayout(&InputLayoutDesc[0],
		InputLayoutDesc.size(),
		ptr_temp->GetBufferPointer(),
		ptr_temp->GetBufferSize(),
		static_cast<ID3D11InputLayout**>(Layout.GetInputLayoutRef()));

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

	if (!CheckForError(hr))
	{
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
	else
	{
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
ID3D11Device * CDevice::GetDevice()
{
	return mptr_Device;
}

ID3D11Device ** CDevice::GetDeviceRef()
{
	return &mptr_Device;
}
#elif
#endif 
