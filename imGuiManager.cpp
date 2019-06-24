#include "imGuiManager.h"
#include <string>

//! for convince
namespace ig = ImGui;

imGuiManager::imGuiManager()
{}

/*!  Highly  Important to free the memory*/
imGuiManager::~imGuiManager()
{

#ifdef USING_DIRECTX
	ImGui_ImplDX11_Shutdown();
#endif // USING_DIRECTX


	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool imGuiManager::Init(CDevice & Device, CDeviaceContext & DeviceContext, HWND& Handle)
{
	bool isSuccesful = false;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& GuiIO = ImGui::GetIO();
	// check for possible error 
	isSuccesful = ImGui_ImplWin32_Init(Handle);
#ifdef USING_DIRECTX
	isSuccesful = ImGui_ImplDX11_Init(Device.GetDevice(), DeviceContext.GetDeviceContext());

#endif // USING_DIRECTX
	ImGui::StyleColorsDark();

	return true;
}

void imGuiManager::MakeBasicWindow(const char * WindowName)
{
	// start a imGui frame
#ifdef USING_DIRECTX
	ImGui_ImplDX11_NewFrame();
#endif // USING_DIRECTX

	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// creates Imgui Test window 
	ImGui::Begin(WindowName);
	ImGui::End();
	// assemble together Draw Data
	ImGui::Render();
	//this is what actually draws the window ONLY work on DirectX 
#ifdef USING_DIRECTX
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // USING_DIRECTX


}

void imGuiManager::MakeWindowFpsAndVertexCount(const char * WindowName, float DeltaTime, int VertexCount)
{
#ifdef USING_DIRECTX
	ImGui_ImplDX11_NewFrame();
#endif // USING_DIRECTX
	// Formating for the FPS counter 
	std::string fpsMessage("FPS ");
	fpsMessage += std::to_string(1 / DeltaTime);

	std::string VertexMessage("Vertex Count # ");
	VertexMessage += std::to_string(VertexCount);

	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// creates Imgui Test window 
	ImGui::Begin(WindowName);

	ImGui::BeginChild("VertexCount", ImVec2(200, 200));
	ImGui::Text(VertexMessage.c_str());
	ImGui::EndChild();

	ImGui::BeginChild("FPS", ImVec2(200, 200));
	ImGui::Text(fpsMessage.c_str());
	ImGui::EndChild();

	ImGui::End();
	// assemble together Draw Data
	ImGui::Render();
	//this is what actually draws the window ONLY work on DirectX 
#ifdef USING_DIRECTX
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // USING_DIRECTX
}
