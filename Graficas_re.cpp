//--------------------------------------------------------------------------------------
// File: Graficas_re.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
bool g_FinishInit = false;
/// --------------------------------MY INCLUDES---------------------------------------///
static float g_Time = 0.0f;
//  Utility
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include "Utility/Timer.h"

// Classes
#include "CDevice.h"
#include "CDeviaceContext.h"
#include "CSwapChian.h"
#include "CBuffer.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CRenderTragetView.h"
#include "ViewPort.h"
#include "CInputLayout.h"
#include "CDepthStencilView.h"
#include "CSampler.h"
#include "CShaderResourceView.h"

// shaders 
#include "CVertexShader.h"
#include "CPixelShader.h"
// standard library
#include <numeric>
#include <algorithm>
// imgui Files 
#include"imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imGuiManager.h"
// assimp
#include <assimp/Importer.hpp>

CDevice MY_Device;// Replaced 
CDeviaceContext MY_DeviceContext;// Replaced 
CSwapChian MY_SwapChain;// Replaced
CBuffer ConstantBufferResize;// Replaced
CBuffer ConstantBufferChangeEveryFrame;// Replaced
CBuffer ConstantBufferNeverChange;// Replaced
CBuffer MY_VertexBuffer;// Replaced 
//! Heres the index-buffer
CBuffer IndexBuffer;// Replaced
CRenderTragetView MY_RenderTragetView;
// ! the camera and values associated with it
CCamera MY_Camera;
CViewPort MY_ViewPort;
//! used to determine the input layout 
CInputLayout MY_InputLayout;// Replaced 
CDepthStencilView MY_DepthStencilView;// Replaced 
//! VertexShader
CVertexShader MY_VertexShader;
// ! Pixel Shader 
CPixelShader MY_PixelShader;
//! shader resource View 
CShaderResourceView MY_ShaderResourceView;
//! My Sampler class 
CSampler MY_Sampler;
/// ImGui Manager 
imGuiManager MY_Gui;
/// This is used to find delta time 
Timer MY_Timer;

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
//

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
LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);
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
	}// 

	CleanupDevice();

	return (int) msg.wParam;
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
	wcex.lpfnWndProc = WindProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR) IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR) IDI_TUTORIAL1);
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
			OutputDebugStringA((char*) pErrorBlob->GetBufferPointer());
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
			D3D11_SDK_VERSION, &sd, MY_SwapChain.GetSwapChianRef(), MY_Device.GetDeviceRef(), &g_featureLevel, MY_DeviceContext.GetDeviceContextRef());
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;
	// Create a render target view

	//ID3D11Texture2D* pBackBuffer = NULL;
	//hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	MY_Gui.Init(MY_Device, MY_DeviceContext, g_hWnd);

	isSuccesful = MY_SwapChain.GetBuffer(0, static_cast<void*>(MY_RenderTragetView.GetBackBufferRef()));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}
	// old code 
	//hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	/// Render traget creation 
	isSuccesful = MY_Device.CreateRenderTargetView(static_cast<void*>(MY_RenderTragetView.GetBackBuffer()),
		static_cast<void*>(MY_RenderTragetView.GetRenderTragetRef()));// &g_pRenderTargetView 

//	RenderTragetView.ReleaseBackBuffer();

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	//pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	// old code 
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
	MY_DepthStencilView.InitDepthStencil2D(height, width, static_cast<int>(DXGI_FORMAT_D24_UNORM_S8_UINT));

	///DepthStencil.InitTexture2D(width, height,
	///	static_cast<int>(DXGI_FORMAT_D24_UNORM_S8_UINT), static_cast<int>(D3D11_BIND_DEPTH_STENCIL));

	isSuccesful = MY_Device.CreateTexture2D(static_cast<void*>(MY_DepthStencilView.GetTexture2DRef()),
		static_cast<void*>(GiveSinglePointer(MY_DepthStencilView.GetTextureDescriptor())));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}
	// Create the depth stencil view
	/// just here for reference
	//D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	//SecureZeroMemory(&descDSV, sizeof(descDSV));
	//descDSV.Format = DepthStencil.GetDescriptor().Format;
	//descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//descDSV.Texture2D.MipSlice = 0;

	/*D3D11_DEPTH_STENCIL_VIEW_DESC *ptr_DepthDescripter =*/
	//g_pDepthStencill
	isSuccesful = MY_Device.CreateDepthStencilView(static_cast<void*>(MY_DepthStencilView.GetTexture2D()),
		static_cast<void*>(GiveSinglePointer(MY_DepthStencilView.ConvertDepthStecilToDx2D())),
		static_cast<void*>(MY_RenderTragetView.GetDepthStencilViewRef()));// &g_pDepthStencilView

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}
	// old code 
	//if (FAILED(hr))
	//	return hr;+

	//g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	MY_DeviceContext.OMSetRenderTargets(1, static_cast<void*>(MY_RenderTragetView.GetRenderTragetRef()),
		static_cast<void*>(MY_RenderTragetView.GetDepthStencilView()));

	// Setup the viewport
	//D3D11_VIEWPORT vp;
	//vp.Width = (FLOAT) width;
	//vp.Height = (FLOAT) height;
	//vp.MinDepth = 0.0f;
	//vp.MaxDepth = 1.0f;
	//vp.TopLeftX = 0;
	//vp.TopLeftY = 0;

	MY_ViewPort.SetupViewPort(height, width, 0, 0);

	// /**/g_pImmediateContext->RSSetViewports(1, &vp);
	MY_DeviceContext.RSSetViewports(1, static_cast<void*>(GiveSinglePointer(MY_ViewPort.GetViewPortRef())));

	// Internaly does the CompileShaderFromFile function
	MY_VertexShader.InitVertexShader(L"Graficas_re.fx", "VS", "vs_4_0");
	// Compile the vertex shader
	//ID3DBlob* p_VertexShaderBlob = nullptr;
	//hr = CompileShaderFromFile(L"Graficas_re.fx", "VS", "vs_4_0", &p_VertexShaderBlob);


	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	isSuccesful = MY_Device.CreateVertexShader(static_cast<void*>(MY_VertexShader.GetVertexShaderData()), static_cast<void*> (&g_pVertexShader));

	MY_InputLayout.ReadShaderDataDX(MY_VertexShader.GetVertexShaderData(), static_cast<int>(D3D11_INPUT_PER_VERTEX_DATA));

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

	///old code 
	// Define the input layout
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//UINT numElements = ARRAYSIZE(layout);
	//
	//MY_InputLayout.ConvertDxToInputLayout(layout, 2);

	auto LayoutConvertion = MY_InputLayout.ConvertInputLayoutToDx();

	// old code
	//hr = g_pd3dDevice->CreateInputLayout(layout, numElements, p_VertexShaderBlob->GetBufferPointer(),
	//	p_VertexShaderBlob->GetBufferSize(), &g_pVertexLayout);

	isSuccesful = MY_Device.CreateInputLayout(static_cast<void*>(&LayoutConvertion[0]), static_cast<void*>(MY_VertexShader.GetVertexShaderData()),
		LayoutConvertion.size(), static_cast<void*>(&g_pVertexLayout));

	//p_VertexShaderBlob->Release();

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Set the input layout
	//g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	MY_DeviceContext.IASetInputLayout(static_cast<void *>(g_pVertexLayout));

	// reads information about the pixel shader 
	isSuccesful = MY_PixelShader.InitPixelShader(L"Graficas_re.fx", "PS", "ps_4_0");

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}
	// Compile the pixel shader
	//ID3DBlob* p_PixelShaderBlob = NULL;
	//hr = CompileShaderFromFile(L"Graficas_re.fx", "PS", "ps_4_0", &p_PixelShaderBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}
	// Create the pixel shader
	/*hr = g_pd3dDevice->CreatePixelShader(p_PixelShaderBlob->GetBufferPointer(), p_PixelShaderBlob->GetBufferSize(), NULL, &g_pPixelShader);*/
	MY_Device.CreatePixelShader(static_cast<void*>(MY_PixelShader.GetPixelShaderData()), static_cast<void*>(MY_PixelShader.GetPixelShaderRef()));

	//	p_PixelShaderBlob->Release();

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
	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer)

	MY_VertexBuffer.IntiVertexBuffer(vertices, 24, 0);

	/*Creates the vertexBuffer*/
	isSuccesful = MY_Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(MY_VertexBuffer.GetBufferRef()),
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

	MY_DeviceContext.IASetVertexBuffers(0, 1,
		static_cast<void*>(MY_VertexBuffer.GetBufferRef()), static_cast<void*>(&stride), static_cast<void*>(&offset));

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

	IndexBuffer.InitIndexBuffer(indices, 36, 0);

	/*!Creates the index buffer */
	MY_Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(IndexBuffer.GetBufferRef()),
		static_cast<void*>(GiveSinglePointer(InitData))
	);

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Set index buffer
	//g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	MY_DeviceContext.IASetIndexBuffer(static_cast<void*>(IndexBuffer.GetBuffer()), static_cast<int>(DXGI_FORMAT_R16_UINT), 0);

	// Set primitive topology
	//g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	MY_DeviceContext.IASetPrimitiveTopology(static_cast<int>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// Create the constant buffers
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBNeverChanges);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ConstantBufferNeverChange.InitConstBuffer(*g_pCBNeverChanges, 0);

	//	hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);

		/*Creates NeverChanges buffer */
	MY_Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(ConstantBufferNeverChange.GetBufferRef()), nullptr);

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	bd.ByteWidth = sizeof(CBChangeOnResize);

	ConstantBufferResize.InitConstBuffer(*g_pCBChangeOnResize, 0);

	MY_Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(ConstantBufferResize.GetBufferRef()), nullptr);

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	bd.ByteWidth = sizeof(CBChangesEveryFrame);

	//	hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);

	MY_Device.CreateBuffer(static_cast<void*>(GiveSinglePointer(bd)),
		static_cast<void*>(ConstantBufferChangeEveryFrame.GetBufferRef()), nullptr);

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Load the Texture
	hr = D3DX11CreateShaderResourceViewFromFile(MY_Device.GetDevice(), L"seafloor.dds", NULL, NULL, MY_ShaderResourceView.GetResourceViewRef(), NULL);
	if (FAILED(hr))
		return hr;

	// Create the sample state
	//D3D11_SAMPLER_DESC sampDesc;
	//SecureZeroMemory(&sampDesc, sizeof(sampDesc));
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//D3D11_FILTER_MIN_MAG_MIP_POINT
	//sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//sampDesc.MinLOD = 0;
	//sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);

	MY_Sampler.SetUpSampler((int) D3D11_FILTER_MIN_MAG_MIP_LINEAR, (int) D3D11_TEXTURE_ADDRESS_WRAP,
		(int) D3D11_TEXTURE_ADDRESS_WRAP, (int) D3D11_TEXTURE_ADDRESS_WRAP,
		(int) D3D11_COMPARISON_NEVER, 0);

	// creates a sampler 
	isSuccesful = MY_Device.CreateSamplerState(static_cast<void*>(GiveSinglePointer(MY_Sampler.ConvertSamplerToDx())),
		static_cast<void*>(MY_Sampler.GetSamplerRef()));

	if (isSuccesful == false)
	{
		HRESULT hr = S_FALSE;
		return hr;
	}

	// Initialize the world matrices
	g_World = XMMatrixIdentity();

	// Initialize the view matrix and Perceptive matrice
	MY_Camera.InitCamara(width, height);

	// old code 
	/*XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);*/

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(MY_Camera.GetViewMatrice());

	MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferNeverChange.GetBuffer()), static_cast<void*>(&cbNeverChanges), 0);
	// old code 
	//g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

	// old code 
	//g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

		// Initialize the projection matrix
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(MY_Camera.GetProyectionMatrice());

	//g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferResize.GetBuffer()), static_cast<void*>(&cbChangesOnResize), 0);

	g_FinishInit = true;
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------

/// ===================HERE IS WIND PROC ===================////
/*! USE THE "wPARAM" it contains the key that's pressed*/
// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT WindowDimentions;

	GetClientRect(g_hWnd, &WindowDimentions);

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	// messages for mouse 
	/*
	MK_LBUTTON          0x0001
	MK_RBUTTON          0x0002
	MK_SHIFT            0x0004
	MK_CONTROL          0x0008
	MK_MBUTTON          0x0010

	/*https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes*/

	POINT mousePointOrigin = { WindowDimentions.right / 2,WindowDimentions.bottom / 2 };
	POINT mousePointEnd;
	XMMATRIX Rotation;
	XMVECTOR AngelVector;

	bool MoveCamara = true;

	D3D11_DEPTH_STENCIL_VIEW_DESC *ptr_DepthDescripter = GiveSinglePointer(MY_DepthStencilView.ConvertDepthStecilToDx2D());

	switch (msg)
	{
	case WM_KEYDOWN:// checks if ANY key was pressed 
		// All of these char's HAVE to be capital letters
		// for this to work 
		MoveCamara = true;
		if (wParam == (WPARAM)'W')
		{
			MY_Camera.MoveCamera(MY_Camera.m_FrontVector);
			MoveCamara = false;
		}
		if (wParam == (WPARAM)'A')
		{
			MY_Camera.MoveCamera(MY_Camera.m_RightVector * -1.0f);

			MoveCamara = false;
		}
		if (wParam == (WPARAM)'S')
		{
			//Camera.MoveTrasfromMatrice(0, 0, -2);
			MY_Camera.MoveCamera(MY_Camera.m_FrontVector * -1.0f);
			MoveCamara = false;
		}
		if (wParam == (WPARAM)'D')
		{
			//Camera.MoveTrasfromMatrice(Camera.m_RightV);
			MY_Camera.MoveCamera(MY_Camera.m_RightVector);
			MoveCamara = false;
		}
		// reset to the default position
		if (wParam == (WPARAM)'R')
		{
			MY_Camera.ResetTrasformMatrice();
			MoveCamara = false;
		}

		if (MoveCamara)
		{
			float Xpos;
			float Ypos;

			GetCursorPos(&mousePointEnd);

			Xpos = mousePointEnd.x - mousePointOrigin.x;
			//	Xpos -= mousePointOrigin.x;

			Ypos = mousePointEnd.y - mousePointOrigin.y;
			//Ypos -= mousePointOrigin.y;

			AngelVector = XMVectorSet(Xpos, -Ypos, 0, 0);

			AngelVector *= 0.00021f;

			MY_Camera.m_At += AngelVector;
			MY_Camera.CoordinateUpdate();
		}

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZING:

		if (g_FinishInit)
		{
			//// destroy all buffer related with the swap-chain , and make all of them nullptr 
			//MY_DepthStencilView.DestoryBuffer();

			//MY_RenderTragetView.DestroyBuffers();

			// tell the swap chine the size of the new window (affecting the buffer )
			MY_SwapChain.ResizeBuffer(WindowDimentions.right, WindowDimentions.bottom, g_hWnd);
			// get a buffer(the one you just made nullptr) and reuse-it as a back buffer again and again and again and AGAIN FOR ALL OF EXISTENCES
		//	MY_SwapChain.GetBuffer(0, static_cast<void*>(MY_RenderTragetView.GetBackBufferRef()));

			// remake your render-target-view 
			//MY_Device.CreateRenderTargetView(static_cast<void*>(MY_RenderTragetView.GetBackBuffer()),
			//	static_cast<void*>(MY_RenderTragetView.GetRenderTragetRef()));
			//// prepare you depth-stencil-view to be remade 
			//MY_DepthStencilView.InitDepthStencil2D(WindowDimentions.bottom, WindowDimentions.right,
			//	static_cast<int>(DXGI_FORMAT_D24_UNORM_S8_UINT));

			//// create a Depth stencil (depth-stencil-views) back buffer 
			//MY_Device.CreateTexture2D(static_cast<void*>(MY_DepthStencilView.GetTexture2DRef()),
			//	static_cast<void*>(GiveSinglePointer(MY_DepthStencilView.GetTextureDescriptor())));

			//// join them 
			//MY_Device.CreateDepthStencilView(static_cast<void*>(MY_DepthStencilView.GetTexture2D()),
			//	static_cast<void*>(GiveSinglePointer(MY_DepthStencilView.ConvertDepthStecilToDx2D())),
			//	static_cast<void*>(MY_RenderTragetView.GetDepthStencilViewRef()));

			//// now set the render-target again .
			//MY_DeviceContext.OMSetRenderTargets(1, static_cast<void*>(MY_RenderTragetView.GetRenderTragetRef()),
			//	static_cast<void*>(MY_RenderTragetView.GetDepthStencilView()));


			//MY_SwapChain.ResizeTarget(WindowDimentions.right, WindowDimentions.bottom);
			//MY_ViewPort.SetupViewPort(WindowDimentions.bottom, WindowDimentions.right, 0, 0);

			//MY_Camera.AlterProyectionMatric(WindowDimentions.right, WindowDimentions.bottom);

			//MY_Camera.CoordinateUpdate();s
		}/// end case 
			//	MY_DeviceContext.RSSetViewports(1, GiveSinglePointer(MY_ViewPort.GetViewPortRef()));

	//			/*            g_pd3dDeviceContext->OMSetRenderTargets(0, 0, 0);

	//						// Release all outstanding references to the swap chain's buffers.
	//						g_pRenderTargetView->Release();

	//						HRESULT hr;
	//						// Preserve the existing buffer count and format.
	//						// Automatically choose the width and height to match the client rect for HWNDs.
	//						hr = g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	//						// Perform error handling here!

	//						// Get buffer and create a render-target-view.
	//						ID3D11Texture2D* pBuffer;
	//						hr = g_pSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D),
	//																				 (void**) &pBuffer );
	//						// Perform error handling here!

	//						hr = g_pd3dDevice->CreateRenderTargetView(pBuffer, NULL,
	//																										 &g_pRenderTargetView);
	//						// Perform error handling here!
	//						pBuffer->Release();

	//						g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL );

	//						// Set up the viewport.
	//						D3D11_VIEWPORT vp;
	//						vp.Width = width;
	//						vp.Height = height;
	//						vp.MinDepth = 0.0f;
	//						vp.MaxDepth = 1.0f;
	//						vp.TopLeftX = 0;
	//						vp.TopLeftY = 0;
	//						g_pd3dDeviceContext->RSSetViewports( 1, &vp );*/

	////	}

	//	break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------


void Render()
{
	// Start Timing to get fps count 
	MY_Timer.StartTiming();

	// Update our time
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		g_Time += (float) XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		g_Time = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	// Rotate cube around the origin
	g_World = XMMatrixRotationY(g_Time);

	// Modify the color
	g_vMeshColor.x = (sinf(g_Time * 1.0f) + 1.0f) * 0.5f;
	g_vMeshColor.y = (cosf(g_Time * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(g_Time * 5.0f) + 1.0f) * 0.5f;

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(MY_Camera.GetViewMatrice());

	MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferNeverChange.GetBuffer()), static_cast<void*>(&cbNeverChanges), 0);

	//g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	MY_DeviceContext.ClearRenderTargetView(static_cast<void*>(MY_RenderTragetView.GetRenderTraget()), ClearColor);

	//
	// Clear the depth buffer to 1.0 (max depth)
	//

	MY_DeviceContext.ClearDepthStencilView(static_cast<void*>(MY_RenderTragetView.GetDepthStencilView()), static_cast<int>(D3D11_CLEAR_DEPTH), 1.0f, 0);
	//g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	float MyCubeColor[4] = { g_vMeshColor.x * -1, 	g_vMeshColor.y * -1, 	g_vMeshColor.z * -1, 1.0f };
	//
	// Update variables that change once per frame
	//
	CBChangesEveryFrame cb;
	cb.mWorld = XMMatrixTranspose(g_World * MY_Camera.GetTrasformMatrice());
	cb.vMeshColor = MyCubeColor;
	//g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferChangeEveryFrame.GetBuffer()), static_cast<void*>(&cb), 0);

	// Initialize the projection matrix
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(MY_Camera.GetProyectionMatrice());

	//g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferResize.GetBuffer()), static_cast<void*>(&cbChangesOnResize), 0);

	//
	// Render the cube
	//g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	//g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
	//g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
	//g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
	//g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	//7g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	//g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	//g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	//g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
	//g_pImmediateContext->DrawIndexed(36, 0, 0);
	//
	MY_DeviceContext.RSSetViewports(1, static_cast<void*>(GiveSinglePointer(MY_ViewPort.GetViewPortRef())));

	MY_DeviceContext.VSSetShader(static_cast<void*>(g_pVertexShader));
	MY_DeviceContext.VSSetConstantBuffers(0, 1, static_cast<void*>(ConstantBufferNeverChange.GetBufferRef()));
	MY_DeviceContext.VSSetConstantBuffers(1, 1, static_cast<void*>(ConstantBufferResize.GetBufferRef()));
	MY_DeviceContext.VSSetConstantBuffers(2, 1, static_cast<void*>(ConstantBufferChangeEveryFrame.GetBufferRef()));
	MY_DeviceContext.PSSetShader(static_cast<void*>(MY_PixelShader.GetPixelShader()));
	MY_DeviceContext.PSSetConstantBuffers(2, 1, static_cast<void*>(ConstantBufferChangeEveryFrame.GetBufferRef()));
	MY_DeviceContext.PSSetShaderResources(0, 1, static_cast<void*>(MY_ShaderResourceView.GetResourceViewRef()));
	MY_DeviceContext.PSSetSamplers(0, 1, static_cast<void*>(MY_Sampler.GetSamplerRef()));
	MY_DeviceContext.DrawIndexed(36, 0, 0);
	/* Making cube rotates */
	g_World *= XMMatrixTranslation(-3, 0, 0);
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.vMeshColor = g_vMeshColor;
	//g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferChangeEveryFrame.GetBuffer()), static_cast<void*>(&cb), 0);
	MY_DeviceContext.DrawIndexed(36, 0, 0);
	/*Make cube that rotate and Scales*/


	FXMVECTOR ScalingVector = XMVectorSet(1, std::fabs(std::sin(g_Time)) * 1.25f, 1, 1);
	g_World = XMMatrixScalingFromVector(ScalingVector);
	g_World *= XMMatrixRotationY(g_Time);
	g_World *= XMMatrixTranslation(0, 2, -2);
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.vMeshColor = g_vMeshColor;
	//g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferChangeEveryFrame.GetBuffer()), static_cast<void*>(&cb), 0);
	MY_DeviceContext.DrawIndexed(36, 0, 0);


	g_World = XMMatrixRotationY(0);
	g_World = XMMatrixScalingFromVector(ScalingVector);
	g_World *= XMMatrixTranslation(3, 0, 0);
	cb.mWorld = XMMatrixTranspose(g_World);
	MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferChangeEveryFrame.GetBuffer()), static_cast<void*>(&cb), 0);
	MY_DeviceContext.DrawIndexed(36, 0, 0);

	/// imGui events ----------------------
	//MY_Gui.MakeBasicWindow("Ventana Bien Pinche meca");


	MY_Timer.EndTiming();
	MY_Gui.MakeWindowFpsAndVertexCount("Stats Window", MY_Timer.GetResultSeconds(), MY_VertexBuffer.GetElementCount());

	//
	// Present our back buffer to our front buffer
	//
	//g_pSwapChain->Present(0, 0);
	MY_SwapChain.Present(0, 0);
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