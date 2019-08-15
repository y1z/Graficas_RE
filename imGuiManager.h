#pragma once
#include "GraphicsLIbsHeader.h"
#include "CDevice.h"
#include "CDeviaceContext.h"

class imGuiManager
{
public:
	imGuiManager();
	~imGuiManager();

	/*! init the imgui library*/
	bool Init(CDevice &Device, CDeviceContext &DeviceContext, HWND& Handle);
	/*! init the imgui library*/
	bool Init(CWindow &Window);
	//! has nothing just the simplest window you can make 
	void MakeBasicWindow(const char* WindowName);
	//! here is a window that contains very specific information 
	void MakeWindowFpsAndVertexCount(const char* WindowName, float DeltaTime, int VertexCount);
};

