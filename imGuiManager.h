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
	bool Init(CDevice &Device, CDeviaceContext &DeviceContext, HWND& Handle);

};

