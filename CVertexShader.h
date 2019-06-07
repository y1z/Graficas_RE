#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"

class CVertexShader
{
public:// construcctor 
	CVertexShader();
	~CVertexShader();
public:// functions 
	bool InitVertexShader(wchar_t *ShaderFile, char * ShaderEntry,char* ShaderVersion);

#ifdef USING_DIRECTX
	ID3D11VertexShader* GetVertexShader();
	ID3D11VertexShader** GetVertexShaderRef();

	ID3DBlob* GetVertexShaderData();
	ID3DBlob** GetVertexShaderDataRef();
#endif // USING_DIRECTX


private:// variables
#ifdef USING_DIRECTX
	ID3D11VertexShader *mptr_vertexShader = nullptr;
	ID3DBlob* mptr_ShaderData = nullptr;
#endif // USING_DIRECTX

};

