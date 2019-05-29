#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"

class VertexShader
{
public:
	VertexShader();
	~VertexShader();
public:// functions 

	bool InitVertexShader(wchar_t *ShaderFile, char * ShaderType,char* ShaderVersion);

public:// variables 

private:
#ifdef USING_DIRECTX
	ID3D11VertexShader *mptr_vertexShader = nullptr;
	ID3DBlob* mptr_ShaderData = nullptr;
#endif // USING_DIRECTX

};

