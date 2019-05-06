//--------------------------------------------------------------------------------------
// File: Graficas_re.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

/// --------------------------------MY INCLUDES---------------------------------------///

//  Utility
#include "Usable_Windows.h"
#include "DirectXHeader.h"

// Classes
#include "CDevice.h"
#include "CDeviaceContext.h"
#include "CSwapChian.h"

CDevice Device;
CDeviaceContext DeviceContext;
CSwapChian SwapChain;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct CBNeverChanges
{
	XMMATRIX mView;
};

struct CBChangeOnResize
{
	XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
	XMMATRIX mWorld;
	XMFLOAT4 vMeshColor;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*                       g_pd3dDevice = NULL;
ID3D11DeviceContext*                g_pImmediateContext = NULL;
IDXGISwapChain*                     g_pSwapChain = NULL;
ID3D11RenderTargetView*             g_pRenderTargetView = NULL;
ID3D11Texture2D*                    g_pDepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11VertexShader*                 g_pVertexShader = NULL;
ID3D11PixelShader*                  g_pPixelShader = NULL;
ID3D11InputLayout*                  g_pVertexLayout = NULL;
ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11SamplerState*                 g_pSamplerLinear = NULL;
XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();
/*! This is for the functions that demand more than 1 pointer*/
template<class T>
T** GiveDoublePointer(T* Base)
{
	T** Result = &Base;
	return Result;
}

/*! This is for functions that need a pointer */
template<class T>
T* GiveSinglePointer(T &Base)
{
	T* Result = &Base;
	return Result;
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return 0;
	}

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}

	CleanupDevice();

	return (int)msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------


/*!
\todo Create own version of this function later down the line.*/
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	/* This is to make sure all the methods of the Device class work */
	bool isSuccesful = false;

	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

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
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, SwapChain.GetSwapChianRef(), Device.GetDeviceRef(), &g_featureLevel, DeviceContext.GetDeviceContextRef());
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	//hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	isSuccesful = SwapChain.GetBuffer(0, static_cast<void*>(&pBackBuffer));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// old code 
	//hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);

	isSuccesful = Device.CreateRenderTargetView(static_cast<void*>(pBackBuffer), static_cast<void*>(&g_pRenderTargetView));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	SecureZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// old code 	
	//hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	//if (FAILED(hr))
	//return hr;

	isSuccesful = Device.CreateTexture2D(static_cast<void*>(&g_pDepthStencil), static_cast<void*>(&descDepth));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	SecureZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC *ptr_DepthDescripter = &descDSV;

	isSuccesful = Device.CreateDepthStencilView(static_cast<void*>(g_pDepthStencil),
		static_cast<void*>(ptr_DepthDescripter),
		static_cast<void*>(&g_pDepthStencilView));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}
	// old code 
	//if (FAILED(hr))
	//	return hr;

	//g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	DeviceContext.OMSetRenderTargets(1, static_cast<void*>(&g_pRenderTargetView), static_cast<void*>(g_pDepthStencilView));

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	//g_pImmediateContext->RSSetViewports(1, &vp);
	DeviceContext.RSSetViewports(1, static_cast<void*>(&vp));

	// Compile the vertex shader
	ID3DBlob* p_VertexShaderBlob = NULL;
	hr = CompileShaderFromFile(L"Graficas_re.fx", "VS", "vs_4_0", &p_VertexShaderBlob);

	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	//hr = g_pd3dDevice->CreateVertexShader(p_VertexShaderBlob->GetBufferPointer(), p_VertexShaderBlob->GetBufferSize(), NULL, &g_pVertexShader);

	isSuccesful = Device.CreateVertexShader(static_cast<void*>(p_VertexShaderBlob), static_cast<void*> (&g_pVertexShader));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	/*if (FAILED(hr))
	{
		p_VertexShaderBlob->Release();
		return hr;
	}*/

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// old code
	//hr = g_pd3dDevice->CreateInputLayout(layout, numElements, p_VertexShaderBlob->GetBufferPointer(),
	//	p_VertexShaderBlob->GetBufferSize(), &g_pVertexLayout);

	isSuccesful = Device.CreateInputLayout(static_cast<void*>(layout), static_cast<void*>(p_VertexShaderBlob),
		numElements, static_cast<void*>(&g_pVertexLayout));

	p_VertexShaderBlob->Release();

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Set the input layout
	//g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	DeviceContext.IASetInputLayout(static_cast<void *>(g_pVertexLayout));

	// Compile the pixel shader
	ID3DBlob* p_PixelShaderBlob = NULL;
	hr = CompileShaderFromFile(L"Graficas_re.fx", "PS", "ps_4_0", &p_PixelShaderBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	/*hr = g_pd3dDevice->CreatePixelShader(p_PixelShaderBlob->GetBufferPointer(), p_PixelShaderBlob->GetBufferSize(), NULL, &g_pPixelShader);*/
	Device.CreatePixelShader(static_cast<void*>(p_PixelShaderBlob), static_cast<void*>(&g_pPixelShader));

	p_PixelShaderBlob->Release();

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}


	// Create vertex buffer
	SimpleVertex vertices[] =
	{
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	SecureZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

	/*Creates the vertexBuffer*/
	isSuccesful = Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(&g_pVertexBuffer),
		static_cast<void*>(GiveSinglePointer(InitData)));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	//g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	DeviceContext.IASetVertexBuffers(0, 1,
		static_cast<void*>(&g_pVertexBuffer), static_cast<void*>(&stride), static_cast<void*>(&offset));

		// Create index buffer
		// Create vertex buffer
		WORD indices[] =
	{
			3,1,0,
			2,1,3,

			6,4,5,
			7,4,6,

			11,9,8,
			10,9,11,

			14,12,13,
			15,12,14,

			19,17,16,
			18,17,19,

			22,20,21,
			23,20,22
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);

	/*Creates the index buffer */
	Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(&g_pIndexBuffer),
		static_cast<void*>(GiveSinglePointer(InitData))
	);

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Set index buffer
	//g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DeviceContext.IASetIndexBuffer(static_cast<void*>(g_pIndexBuffer), static_cast<int>(DXGI_FORMAT_R16_UINT), 0);

	// Set primitive topology
	//g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext.IASetPrimitiveTopology(static_cast<int>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// Create the constant buffers
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBNeverChanges);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	//	hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
		/*Creates NeverChanges buffer */
	Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(&g_pCBNeverChanges), nullptr);

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	bd.ByteWidth = sizeof(CBChangeOnResize);

	Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(&g_pCBChangeOnResize), nullptr);

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	bd.ByteWidth = sizeof(CBChangesEveryFrame);

	//	hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);

	Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(&g_pCBChangesEveryFrame), nullptr);

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Load the Texture
	hr = D3DX11CreateShaderResourceViewFromFile(Device.GetDeviceTemp(), L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
	if (FAILED(hr))
		return hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	SecureZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);

	isSuccesful = Device.CreateSamplerState(static_cast<void*>(GiveSinglePointer(sampDesc)),
		static_cast<void*>(&g_pSamplerLinear));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Initialize the world matrices
	g_World = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(g_View);


	DeviceContext.UpdateSubresource(static_cast<void*>(g_pCBNeverChanges),static_cast<void*>(&cbNeverChanges),0);
	//g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);

	//g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	DeviceContext.UpdateSubresource(static_cast<void*>(g_pCBChangeOnResize), static_cast<void*>(&cbChangesOnResize),0);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();

	if (g_pSamplerLinear) g_pSamplerLinear->Release();
	if (g_pTextureRV) g_pTextureRV->Release();
	if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
	if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
	if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pDepthStencil) g_pDepthStencil->Release();
	if (g_pDepthStencilView) g_pDepthStencilView->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
	// Update our time
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	// Rotate cube around the origin
	g_World = XMMatrixRotationY(t);

	// Modify the color
	g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
	g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

	//g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	
	DeviceContext.ClearRenderTargetView(static_cast<void*>(g_pRenderTargetView), ClearColor);

	//
	// Clear the depth buffer to 1.0 (max depth)
	//

	DeviceContext.ClearDepthStencilView(static_cast<void*>(g_pDepthStencilView), static_cast<int>(D3D11_CLEAR_DEPTH),1.0f,0);
	//g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//
	// Update variables that change once per frame
	//
	CBChangesEveryFrame cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.vMeshColor = g_vMeshColor;
	//g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	DeviceContext.UpdateSubresource(static_cast<void*>(g_pCBChangesEveryFrame), static_cast<void*>(&cb), 0);

	//
	// Render the cube
	//g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	//g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
	//g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
	//g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	//7g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	//g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	//g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	//g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
	//g_pImmediateContext->DrawIndexed(36, 0, 0);
	//
	DeviceContext.VSSetShader(static_cast<void*>(g_pVertexShader));
	DeviceContext.VSSetConstantBuffers(0, 1, static_cast<void*>(&g_pCBNeverChanges));
	DeviceContext.VSSetConstantBuffers(1, 1, static_cast<void*>(&g_pCBChangeOnResize));
	DeviceContext.VSSetConstantBuffers(2, 1, static_cast<void*>(&g_pCBChangesEveryFrame));
	DeviceContext.PSSetShader(static_cast<void*>(g_pPixelShader));
	DeviceContext.PSSetConstantBuffers(2, 1, static_cast<void*>(&g_pCBChangesEveryFrame));
	DeviceContext.PSSetShaderResources(0, 1, static_cast<void*>(&g_pTextureRV));
	DeviceContext.PSSetSamplers(0, 1, static_cast<void*>(&g_pSamplerLinear));
	DeviceContext.DrawIndexed(36, 0, 0);
	//
	// Present our back buffer to our front buffer
	//
	//g_pSwapChain->Present(0, 0);
	SwapChain.Present(0, 0);
}

