#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"

class CPixelShader
{
public:
	CPixelShader();
	~CPixelShader();

	bool InitPixelShader(wchar_t *ShaderFile, char * ShaderEntry, char* ShaderVersion);
#ifdef USING_DIRECTX
	ID3D11PixelShader * GetPixelShader();
	ID3D11PixelShader **GetPixelShaderRef();

	ID3DBlob* GetPixelShaderData();
	ID3DBlob** GetPixelShaderDataRef();

	ID3D11PixelShader* mptr_PixelShader = nullptr;
	ID3DBlob* mptr_ShaderData = nullptr;
#endif // USING_DIRECTX

};

