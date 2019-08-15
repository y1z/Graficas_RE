#pragma once
#include "Usable_Windows.h"
#include  "GraphicsLIbsHeader.h"
class CDevice;
class CDeviceContext;

class CShaderResourceView
{
public:// constructor 
	CShaderResourceView();
	~CShaderResourceView();
public:// functions 
#ifdef USING_DIRECTX
	ID3D11ShaderResourceView* GetResourceView();
	ID3D11ShaderResourceView** GetResourceViewRef();
#endif // USING_DIRECTX
	bool CreateShaderResourceViewFromFile(CDevice &Device, CDeviceContext &DContext, wchar_t *FileName);

	bool CreateShaderResourceViewFromFile(CDevice &Device, CDeviceContext &DContext, const char *FileName);
#ifdef USING_OPEN_GL
	uint32_t GetResuorce() const;
#endif // USING_OPENG_GL


private:// variables 
#ifdef USING_DIRECTX
	ID3D11ShaderResourceView* mptr_ResourceView = nullptr;
#else 
	uint32_t m_ResourceID = 0;
	unsigned char *mpr_Resouce = nullptr;
#endif // USING_DIRECTX



};

