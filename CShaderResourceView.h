#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
class CDevice;

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
	bool CreateShaderResourceViewFromFile(CDevice &Device,wchar_t *FileName);

private:// variables 
#ifdef USING_DIRECTX
	ID3D11ShaderResourceView* mptr_ResourceView = nullptr;

#endif // USING_DIRECTX

};

