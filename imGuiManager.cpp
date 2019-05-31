#include "imGuiManager.h"


imGuiManager::imGuiManager()
{}


imGuiManager::~imGuiManager()
{}

bool imGuiManager::Init(CDevice & Device, CDeviaceContext & DeviceContext, HWND& Handle)
{
#ifdef USING_DIRECTX

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& GuiIO = ImGui::GetIO();
	ImGui_ImplWin32_Init(Handle);
	ImGui_ImplDX11_Init(Device.GetDeviceTemp(), DeviceContext.GetDeviceContext());
	ImGui::StyleColorsDark();
#endif // USING_DIRECTX



	return false;
}
