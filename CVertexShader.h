#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include <string>
#include <cinttypes>

class CVertexShader
{
public:// construcctor 
	CVertexShader();
	~CVertexShader();
public:// functions 
	bool InitVertexShader(wchar_t *ShaderFile, char * ShaderEntry, char* ShaderVersion);

#if USING_OPEN_GL
	bool InitVertexShader(const char *ShaderFile, const char *ShaderEntry, const char *ShaderVersion);

#endif // USING_OPEN_GL


#ifdef USING_DIRECTX
	ID3D11VertexShader* GetVertexShader();
	ID3D11VertexShader** GetVertexShaderRef();

	ID3DBlob* GetVertexShaderData();
	ID3DBlob** GetVertexShaderDataRef();
#elif USING_OPEN_GL

#endif // USING_DIRECTX
	uint32_t m_VertexShaderID = 0;
	std::string m_Shader;
private:// variables
#ifdef USING_DIRECTX
	ID3D11VertexShader *mptr_vertexShader = nullptr;
	ID3DBlob* mptr_ShaderData = nullptr;
#elif USING_OPEN_GL

#endif // USING_DIRECTX

};

