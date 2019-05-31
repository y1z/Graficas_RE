#pragma once
#include"imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#ifdef USING_DIRECTX
#include "imgui/imgui_impl_dx11.h"
#endif // USING_DIRECTX
#include "CDevice.h"
#include "CDeviaceContext.h"


class imGuiManager
{

public:
	imGuiManager();
	~imGuiManager();
public:
	/*! start the imgui class*/
	bool Init(CDevice &Device, CDeviaceContext &DeviceContext, HWND& Handle);
	//! has nothing just the simplest window you can make 
	void MakeBasicWindow(const char* WindowName);
	//! here is a window that contains very specific information 
	void MakeWindowFpsAndVertexCount(const char* WindowName, float DeltaTime, int VertexCount);

	

};

