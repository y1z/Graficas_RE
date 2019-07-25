#pragma once
#include  "GraphicsLibsHeader.h"
#include <string>
#ifdef USING_DIRECTX
#include "DirectXHeader.h"
#endif // USING_DIRECTX

class CPixelShader
{
public:
	CPixelShader();
	~CPixelShader();

	bool InitPixelShader(wchar_t *ShaderFile, char * ShaderEntry, char* ShaderVersion);
	bool InitPixelShader(const char *ShaderFile);

#ifdef USING_DIRECTX
	ID3D11PixelShader * GetPixelShader();
	ID3D11PixelShader **GetPixelShaderRef();

	ID3DBlob* GetPixelShaderData();
	ID3DBlob** GetPixelShaderDataRef();

	ID3D11PixelShader* mptr_PixelShader = nullptr;
	ID3DBlob* mptr_ShaderData = nullptr;
#elif USING_OPEN_GL
	unsigned int m_PixelShaderID;
	std::string m_Shader;
#endif // USING_DIRECTX

};

