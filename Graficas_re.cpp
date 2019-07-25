// OpenGL 
#include "GraphicsLIbsHeader.h"
//--------------------------------------------------------------------------------------
// File: Graficas_re.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define MODEL_LOAD 1

unsigned int g_ShaderProgram{ 0 };

bool g_FinishInit = false;
/// --------------------------------MY INCLUDES---------------------------------------///
//  Utility
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include "Utility/Timer.h"
// Classes
#include "CDevice.h"
#include "CDeviaceContext.h"
#include "CSwapChian.h"
#include "CBuffer.h"
//Window
#include "CWindow.h"

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
// imgui Files 
#include"imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imGuiManager.h"
// structs 
#include "Structs.h"
#include "Custom_Structs.h"
// model 
#include "CModel.h"
//standered includes 
#include <array>
#include <cassert>

CWindow MY_Window;
CDevice MY_Device;// Replaced 
CDeviceContext MY_DeviceContext;// Replaced  
CSwapChian MY_SwapChain;// Replaced
CBuffer ConstantBufferResize;// Replaced
CBuffer ConstantBufferChangeEveryFrame;// Replaced
CBuffer ConstantBufferNeverChange;// Replaced
CBuffer MY_VertexBuffer;// Replaced 
//! Heres the index-buffer
CBuffer MY_IndexBuffer;// Replaced
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
//! used to load models 
CModel MY_Model;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = nullptr;
HWND                                g_hWnd = nullptr;
//D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
//D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
//ID3D11Device*                       g_pd3dDevice = NULL;
//ID3D11DeviceContext*                g_pImmediateContext = NULL;
//IDXGISwapChain*                     g_pSwapChain = NULL;
//ID3D11RenderTargetView*             g_pRenderTargetView = NULL;
//ID3D11Texture2D*                    g_pDepthStencil = NULL;
//ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
//ID3D11VertexShader*                 g_pVertexShader = NULL;
//ID3D11PixelShader*                  g_pPixelShader = NULL;
//ID3D11InputLayout*                  g_pVertexLayout = NULL;
//ID3D11Buffer*                       g_pVertexBuffer = NULL;
//ID3D11Buffer*                       g_pIndexBuffer = NULL;
//ID3D11Buffer*                       g_pCBNeverChanges = NULL;
//ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
//ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;
//ID3D11ShaderResourceView*           g_pTextureRV = NULL;
//ID3D11SamplerState*                 g_pSamplerLinear = NULL;
#ifdef USING_DIRECTX
XMMATRIX                            g_World;
#endif // USING_DIRECTX

//XMMATRIX                            g_View;
//XMMATRIX                            g_Projection;
//XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
glm::vec4	g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
glm::mat4	GL_World(1.0f);
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT Preamble();
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


static bool GlAttachShaders(CVertexShader& vs, CPixelShader &ps)
{
#if USING_OPEN_GL

	GlRemoveAllErrors();
	g_ShaderProgram = glCreateProgram();
	glAttachShader(g_ShaderProgram, vs.m_VertexShaderID);
	glAttachShader(g_ShaderProgram, ps.m_PixelShaderID);
	glLinkProgram(g_ShaderProgram);
	glValidateProgram(g_ShaderProgram);

	int Status;
	glGetProgramiv(g_ShaderProgram, GL_VALIDATE_STATUS, &Status);
	//if(Status == GL_FALSE)
	//{
	//	assert(true == false && "Something whent wrong ");
	//}

	if (GlCheckForError())
	{
		return false;
	}

	return true;
#endif // USING_OPEN_GL

}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/* Make the window's context current */
	//glfwMakeContextCurrent(window);
	ptr_WindProc ptr_Proc = &WindProc;
	/// NEED TO MAKE A RENDER CONTEXT before initiating 
#if USING_OPEN_GL
	if (glfwInit() != GLFW_TRUE)
	{ return -1; }
#endif // USING_OPEN_GL

	if (!MY_Window.InitWindow(hInstance, ptr_Proc))
	{
		return -1;
	}

	// create after render context 
#if USING_OPEN_GL
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}
#endif

	if ((Preamble() == S_FALSE))
	{
		MessageBox(NULL,
			L"Error Ocurred with the function Preamble ", L"Error", MB_OK);
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

#if USING_OPEN_GL
	/* Terminate the library*/
	glfwTerminate();
#endif // USING_OPEN_GL

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
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT Preamble()
{
	/* This is to make sure all the methods of the Device class work */
	bool isSuccesful = false;

	HRESULT hr = S_OK;

	isSuccesful = MY_Device.InitDevice(MY_SwapChain, MY_DeviceContext, MY_Window);
	// Create a render target view

	//ID3D11Texture2D* pBackBuffer = NULL;
	//hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
#if USING_DIRECTX
	MY_Gui.Init(MY_Device, MY_DeviceContext, MY_Window.GetHandlerRef());
#endif // USING_DIRECTX

	isSuccesful = MY_SwapChain.GetBuffer(0, MY_RenderTragetView);

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}
	// old code 
	//hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	/// Render target creation 
	isSuccesful = MY_Device.CreateRenderTargetView(MY_RenderTragetView);// &g_pRenderTargetView 

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	// old code 
	//D3D11_TEXTURE2D_DESC descDepth;
	//SecureZeroMemory(&descDepth, sizeof(descDepth));
	//descDepth.Width = width;
	//descDepth.Height = height;
	//descDepth.MipLevels = 1;
	//descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//descDepth.SampleDesc.Count = 1;
	//descDepth.SampleDesc.Quality = 0;
	//descDepth.Usage = D3D11_USAGE_DEFAULT;
	//descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//descDepth.CPUAccessFlags = 0;
	//descDepth.MiscFlags = 0;

	// old code 	
	//hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	//if (FAILED(hr))
	//return hr;
#if USING_DIRECTX
	MY_DepthStencilView.InitDepthStencil2D(MY_Window.GetHeight(), MY_Window.GetWidth(), static_cast<int>(DXGI_FORMAT_D24_UNORM_S8_UINT));
#elif USING_OPEN_GL

	MY_DepthStencilView.InitDepthStencil2D(MY_Window.GetHeight(), MY_Window.GetWidth(), GL_DEPTH_TEST);

#endif // USING_DIRECTX
	///DepthStencil.InitTexture2D(width, height,
	///	static_cast<int>(DXGI_FORMAT_D24_UNORM_S8_UINT), static_cast<int>(D3D11_BIND_DEPTH_STENCIL));

	isSuccesful = MY_Device.CreateTexture2D(MY_DepthStencilView.GetTexture2DRef());

	if (isSuccesful == false)
	{
		hr = S_FALSE;
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
	//isSuccesful = MY_Device.CreateDepthStencilView(static_cast<void*>(MY_DepthStencilView.GetTexture2D()),
	//	static_cast<void*>(GiveSinglePointer(MY_DepthStencilView.ConvertDepthStecilToDx2D())),
	//	static_cast<void*>(MY_DepthStencilView.GetDepthStencilViewRef()));// &g_pDepthStencilView

	isSuccesful = MY_Device.CreateDepthStencilView(MY_DepthStencilView);

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}
	// old code 
	//if (FAILED(hr))
	//	return hr;

	//g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	MY_DeviceContext.OMSetRenderTargets(1, MY_RenderTragetView, MY_DepthStencilView);

	// Setup the viewport
	MY_ViewPort.SetupViewPort(MY_Window.GetHeight(), MY_Window.GetWidth(), 0, 0);

	// /**/g_pImmediateContext->RSSetViewports(1, &vp);
	MY_DeviceContext.RSSetViewports(1, MY_ViewPort);

	wchar_t *ShaderFile = L"shaders/Lambert.fx";

	// Internally does the CompileShaderFromFile function
#if USING_DIRECTX
	// used to be vs_4_0
	MY_VertexShader.InitVertexShader(ShaderFile, "vs_main", "vs_4_0");
#elif USING_OPEN_GL
	MY_VertexShader.InitVertexShader("Shaders/basic.vert");
#endif // USING_DIRECTX

	// Compile the vertex shader
	//ID3DBlob* p_VertexShaderBlob = nullptr;
	//hr = CompileShaderFromFile(L"Graficas_re.fx", "VS", "vs_4_0", &p_VertexShaderBlob);

	if (FAILED(hr) || !isSuccesful)
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	isSuccesful = MY_Device.CreateVertexShader(MY_VertexShader);

#if USING_DIRECTX
	MY_InputLayout.ReadShaderDataDX(MY_VertexShader, static_cast<int>(D3D11_INPUT_PER_VERTEX_DATA));
#endif // USING_DIRECTX

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

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

	// old code
	//hr = g_pd3dDevice->CreateInputLayout(layout, numElements, p_VertexShaderBlob->GetBufferPointer(),
	//	p_VertexShaderBlob->GetBufferSize(), &g_pVertexLayout);

	isSuccesful = MY_Device.CreateInputLayout(MY_InputLayout, MY_VertexShader);
	//p_VertexShaderBlob->Release();

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	// Set the input layout
	//g_pImmediateContext->IASetInputLayout(g_pVertexLayout); 
	MY_DeviceContext.IASetInputLayout(MY_InputLayout);

	// reads information about the pixel shader 
#if USING_DIRECTX
		// used to be ps_4_0
	isSuccesful = MY_PixelShader.InitPixelShader(ShaderFile, "ps_main", "ps_4_0");
#elif USING_OPEN_GL
	isSuccesful = MY_PixelShader.InitPixelShader("Shaders/basic.frag");
#endif // USING_DIRECTX

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	// Compile the pixel shader
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	MY_Device.CreatePixelShader(MY_PixelShader);

#ifdef USING_OPEN_GL
	GlAttachShaders(MY_VertexShader, MY_PixelShader);
#endif // USING_OPEN_GL

#ifndef MODEL_LOAD
	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	// Create vertex buffer
	VertexWithTexture Vertices[] =
	{
		{glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec2(0.0f, 0.0f) },
		{glm::vec3(1.0f, 1.0f, -1.0f) ,glm::vec2(1.0f, 0.0f) },
		{glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
		{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{glm::vec3(-1.0f, -1.0f, -1.0),glm::vec2(0.0f, 0.0f) },
		{glm::vec3(1.0f, -1.0f, -1.0f),glm::vec2(1.0f, 0.0f) },
		{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec2(0.0f, 1.0f) },

		{glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec2(0.0f, 0.0f) },
		{glm::vec3(-1.0f, -1.0f, -1.0),glm::vec2(1.0f, 0.0f) },
		{glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec2(1.0f, 1.0f) },
		{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{glm::vec3(1.0f, -1.0f, -1.0f),glm::vec2(1.0f, 0.0f) },
		{glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },

		{glm::vec3(-1.0f, -1.0f, -1.0),glm::vec2(0.0f, 0.0f) },
		{glm::vec3(1.0f, -1.0f, -1.0f),glm::vec2(1.0f, 0.0f) },
		{glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec2(0.0f, 1.0f) },

		{glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec2(0.0f, 0.0f) },
		{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		{glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
		{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
	};

	MY_VertexBuffer.InitVertexBuffer(Vertices, 24, 0, sizeof(VertexWithTexture));

	/*Creates the vertexBuffer*/
	isSuccesful = MY_Device.CreateBuffer(MY_VertexBuffer);

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	//g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	MY_DeviceContext.IASetVertexBuffers(0, 1, MY_VertexBuffer, MY_VertexBuffer.GetStride(), MY_VertexBuffer.GetOffset());

	// Create index buffer
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

	MY_IndexBuffer.InitIndexBuffer(static_cast<void*>(&indices), 36, 0, sizeof(WORD));

	/*!Creates the index buffer */
	MY_Device.CreateBuffer(MY_IndexBuffer);

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}
	// Set index buffer
	//g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	/// DXGI_FORMAT_R16_UINT = 57
	MY_DeviceContext.IASetIndexBuffer(MY_IndexBuffer, 57, 0);

#else
	MY_Model.LoadModelFromFile("Models/Basic/Esfera.fbx", MY_Device);
	//MY_Model.CreateModelBuffers(MY_Device);

#endif //MODEL_LOADs
	// Set primitive topology
	//g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#if USING_DIRECTX
	MY_DeviceContext.IASetPrimitiveTopology(static_cast<int>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	glm::mat4x4 NeverChangeBuffer;

	ConstantBufferNeverChange.InitConstBuffer(static_cast<void*>(&NeverChangeBuffer), 0, sizeof(NeverChangeBuffer), g_ShaderProgram, 0);
#endif // USING DIRECTX

	// Create the constant buffers
//bd.Usage = D3D11_USAGE_DEFAULT;
//bd.ByteWidth = sizeof(CBNeverChanges);
//bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//bd.CPUAccessFlags = 0;

#ifdef USING_OPEN_GL

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#endif // USING_OPEN_GL

	/*Creates NeverChanges buffer */
	isSuccesful = MY_Device.CreateBuffer(ConstantBufferNeverChange);

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	//	bd.ByteWidth = sizeof(CBChangeOnResize);

	GlChangeOnResizeBuf ChangeOnResize;
	ConstantBufferResize.InitConstBuffer(static_cast<void*>(&ChangeOnResize), 0, sizeof(ChangeOnResize), g_ShaderProgram, 1);

	isSuccesful = MY_Device.CreateBuffer(ConstantBufferResize);

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	//	bd.ByteWidth = sizeof(CBChangesEveryFrame);
	//CBChangesEveryFrame ChangeEveryFrame;
	GlChangesEveryFrameBuf ChangeEveryFrame;
	//	hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
	ConstantBufferChangeEveryFrame.InitConstBuffer(static_cast<void*>(&ChangeEveryFrame), 0, sizeof(ChangeEveryFrame), g_ShaderProgram, 2);

	isSuccesful = MY_Device.CreateBuffer(ConstantBufferChangeEveryFrame);

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

#if USING_DIRECTX
	isSuccesful = MY_ShaderResourceView.CreateShaderResourceViewFromFile(MY_Device, L"seafloor.dds");
#endif // USING_DIRECTX

	// Load the Texture
	//hr = D3DX11CreateShaderResourceViewFromFile(MY_Device.GetDevice(), L"seafloor.dds", NULL, NULL, MY_ShaderResourceView.GetResourceViewRef(), NULL);
	//if (FAILED(hr))
	//	return hr;
	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	//hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);

#if USING_DIRECTX
	MY_Sampler.SetUpSampler((int) D3D11_FILTER_MIN_MAG_MIP_LINEAR, (int) D3D11_TEXTURE_ADDRESS_WRAP,
		(int) D3D11_TEXTURE_ADDRESS_WRAP, (int) D3D11_TEXTURE_ADDRESS_WRAP,
		(int) D3D11_COMPARISON_NEVER, 0);
#else

#endif // USING_DIRECTX
	// creates a sampler 
	isSuccesful = MY_Device.CreateSamplerState(MY_Sampler);

	if (isSuccesful == false)
	{
		hr = S_FALSE;
		return hr;
	}

	// Initialize the view matrix and Perceptive matrice
	MY_Camera.InitCamara(MY_Window.GetWidth(), MY_Window.GetHeight());

	// old code 
	/*XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);*/

	//CBNeverChanges cbNeverChanges;
	//cbNeverChanges.mView = XMMatrixTranspose(MY_Camera.GetViewMatrice());
	//// HERE IS FOR UPDATING RESROUCES 
	//MY_DeviceContext.UpdateSubresource(ConstantBufferNeverChange, static_cast<void*>(&cbNeverChanges), 0);
	//// old code 
	////g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
	//
	//// old code 
	////g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);
	//
	//	// Initialize the projection matrix
	//CBChangeOnResize cbChangesOnResize;
	//cbChangesOnResize.mProjection = XMMatrixTranspose(MY_Camera.GetProyectionMatrice());
	//
	////g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	//MY_DeviceContext.UpdateSubresource(ConstantBufferResize, static_cast<void*>(&cbChangesOnResize), 0);
	//
	g_FinishInit = true;
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
	static float Time = 0.00f;
	// Start Timing to get fps count 
	MY_Timer.StartTiming();

	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	Time = (dwTimeCur - dwTimeStart) / 1000.0f;

	// Rotate cube around the origin
	//g_World = XMMatrixRotationY(Time);

	// Modify the color
	g_vMeshColor.x = (sinf(Time * 1.0f) + 1.0f) * 0.5f;
	g_vMeshColor.y = (cosf(Time * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(Time * 5.0f) + 1.0f) * 0.5f;

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

#if USING_DIRECTX
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(MY_Camera.GetViewMatrice());

	MY_DeviceContext.UpdateSubresource(ConstantBufferNeverChange, static_cast<void*>(&cbNeverChanges), 0);
#endif // USING_DIRECTX

	//g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	MY_DeviceContext.ClearRenderTargetView(MY_RenderTragetView, ClearColor);

	//
	// Clear the depth buffer to 1.0 (max depth)
	//

	MY_DeviceContext.ClearDepthStencilView(MY_DepthStencilView, 1, 1.0f, 0);
	//g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	float MyCubeColor[4] = { g_vMeshColor.x, 	g_vMeshColor.y , 	g_vMeshColor.z, 1.0f };
	//
	// Update variables that change once per frame
	//
#if USING_DIRECTX
	CBChangesEveryFrame cb;
	cb.mWorld = XMMatrixTranspose(/*GL_World **/ MY_Camera.GetTrasformMatrice());
	cb.vMeshColor = MyCubeColor;
	//g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	MY_DeviceContext.UpdateSubresource(ConstantBufferChangeEveryFrame, static_cast<void*>(&cb), 0);

	// Initialize the projection matrix
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(MY_Camera.GetProyectionMatrice());

	//g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	MY_DeviceContext.UpdateSubresource(ConstantBufferResize, static_cast<void*>(&cbChangesOnResize), 0);
#endif // USING_DIRECTX
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
	MY_DeviceContext.RSSetViewports(1, MY_ViewPort);

	MY_DeviceContext.VSSetShader(MY_VertexShader);
	MY_DeviceContext.VSSetConstantBuffers(0, 1, &ConstantBufferNeverChange);
	MY_DeviceContext.VSSetConstantBuffers(1, 1, &ConstantBufferResize);
	MY_DeviceContext.VSSetConstantBuffers(2, 1, &ConstantBufferChangeEveryFrame);


	std::array<CBuffer *, 3> ConstBufferArray = {
		&ConstantBufferNeverChange,
		&ConstantBufferResize,
		&ConstantBufferChangeEveryFrame
	};

	MY_DeviceContext.PSSetShader(MY_PixelShader);
	MY_DeviceContext.PSSetConstantBuffers(2, 1, &ConstantBufferChangeEveryFrame);
	MY_DeviceContext.PSSetShaderResources(0, 1, MY_ShaderResourceView);
	MY_DeviceContext.PSSetSamplers(0, 1, MY_Sampler);

#ifndef	MODEL_LOAD
	MY_DeviceContext.DrawIndexed(MY_IndexBuffer);
#endif 
#if MODEL_LOAD

	MY_Model.DrawAllMeshes(MY_DeviceContext, ConstBufferArray, MY_Camera);
	MY_SwapChain.Present(0, 0);
	//MY_DeviceContext.DrawIndexed(MY_IndexBuffer.GetElementCount(), 0, 0);
	/* Making cube rotates */
	//g_World *= XMMatrixTranslation(0, 0, -20);
	//cb.mWorld = XMMatrixTranspose(g_World);
	////cb.vMeshColor = g_vMeshColor;
	////g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	////MY_DeviceContext.UpdateSubresource(ConstantBufferChangeEveryFrame, static_cast<void*>(&cb), 0);
	////MY_DeviceContext.DrawIndexed(MY_Model.m_Meshs[0].mptr_IndexBuffer.GetElementCount(), 0, 0);
	/////*Make cube that rotate and Scales*/
	//FXMVECTOR ScalingVector = { 1.0f,1.0f,1.0f,3.0f };
	//
	////FXMVECTOR ScalingVector = XMVectorSet(1, std::fabs(std::sin(Time)) * 1.25f, 1, 1);
	//g_World = XMMatrixScalingFromVector(ScalingVector);
	////g_World *= XMMatrixRotationY(Time);
	////g_World *= XMMatrixTranslation(0, 2, -2);
	////cb.mWorld = XMMatrixTranspose(g_World);
	////cb.vMeshColor = g_vMeshColor;
	//////g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	////MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferChangeEveryFrame.GetBuffer()), static_cast<void*>(&cb), 0);
	////MY_DeviceContext.DrawIndexed(MY_Model.m_Meshs[0].mptr_IndexBuffer.GetElementCount(), 0, 0);
	//  
#endif // MODEL_LOAD



	//g_World = XMMatrixRotationY(0);
	//g_World = XMMatrixScalingFromVector(ScalingVector);
	//g_World *= XMMatrixTranslation(3, 0, 0);
	//cb.mWorld = XMMatrixTranspose(g_World);
	//MY_DeviceContext.UpdateSubresource(static_cast<void*>(ConstantBufferChangeEveryFrame.GetBuffer()), static_cast<void*>(&cb), 0);
	//MY_DeviceContext.DrawIndexed(MY_Model.m_Meshs[0].mptr_IndexBuffer.GetElementCount(), 0, 0);

	/// imGui events ----------------------
	MY_Timer.EndTiming();
#ifdef USING_DIRECTX && MODEL_LOAD
	MY_Gui.MakeWindowFpsAndVertexCount("Stats Window", MY_Timer.GetResultSeconds(), MY_Model.GetTotalVertices());
#elif USING_DIRECTX
	MY_Gui.MakeWindowFpsAndVertexCount("Stats Window", MY_Timer.GetResultSeconds(), MY_VertexBuffer.GetElementCount());
#elif USING_OPEN_GL

#endif // USING_DIRECTX

	// Present our back buffer to our front buffer
	//
	//g_pSwapChain->Present(0, 0);
#ifdef USING_OPEN_GL
	glPushMatrix();
	glTranslatef(MY_Window.GetWidth() / 2, MY_Window.GetHeight() / 2, -500);
	glBindBuffer(GL_ARRAY_BUFFER, MY_VertexBuffer.m_BufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MY_IndexBuffer.m_BufferID);

	glDrawElements(GL_TRIANGLES, MY_VertexBuffer.GetElementCount(), GL_UNSIGNED_SHORT, nullptr);
	MY_SwapChain.Present(0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif // using_open_gl 
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	//if (g_pImmediateContext) g_pImmediateContext->ClearState();

	//if (g_pSamplerLinear) g_pSamplerLinear->Release();
	//if (g_pTextureRV) g_pTextureRV->Release();
	//if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
	//if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
	//if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
	//if (g_pVertexBuffer) g_pVertexBuffer->Release();
	//if (g_pIndexBuffer) g_pIndexBuffer->Release();
	//if (g_pVertexLayout) g_pVertexLayout->Release();
	//if (g_pVertexShader) g_pVertexShader->Release();
//	if (g_pPixelShader) g_pPixelShader->Release();
	//if (g_pDepthStencil) g_pDepthStencil->Release();
//	if (g_pDepthStencilView) g_pDepthStencilView->Release();
	//if (g_pRenderTargetView) g_pRenderTargetView->Release();
//	if (g_pSwapChain) g_pSwapChain->Release();
	//if (g_pImmediateContext) g_pImmediateContext->Release();
	//if (g_pd3dDevice) g_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------

/*! USE THE "wPARAM" it contains the key that's pressed*/
 // Win32 message handler
#if USING_DIRECTX
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // USING_DIRECTX
LRESULT CALLBACK WindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT WindowDimentions;

#if USING_DIRECTX
	GetWindowRect(MY_Window.GetHandlerRef(), &WindowDimentions);

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
#endif // USING_DIRECTX
	// messages for mouse 
	/*
	MK_LBUTTON          0x0001
	MK_RBUTTON          0x0002
	MK_SHIFT            0x0004
	MK_CONTROL          0x0008
	MK_MBUTTON          0x0010

	/*https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes*/

#if USING_DIRECTX

	POINT mousePointOrigin = { WindowDimentions.right / 2,WindowDimentions.bottom / 2 };
	POINT mousePointEnd;
	XMMATRIX Rotation;
	XMVECTOR AngelVector;

	bool MoveCamara = true;
	/*******************USE THIS TO GET THE WIDOW SIZE ************/
//get the first 16-bits of the integer 
	int ClientX = LOWORD(lParam);
	// get the remaining 16-bits 
	int ClienY = HIWORD(lParam);
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
		///	MY_SwapChain.ResizeBuffer(WindowDimentions.right, WindowDimentions.bottom, MY_Window.GetHandlerRef());
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
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
#endif // USING_DIRECTX
	return 0;
}