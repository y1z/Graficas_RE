#include "CVertexShader.h"
#include "Utility/ErrorHandlingGrafics.h"

CVertexShader::CVertexShader()
{}


CVertexShader::~CVertexShader()
{}

bool CVertexShader::InitVertexShader(wchar_t * ShaderFile, char * ShaderEntry, char * ShaderVersion)
{
#ifdef USING_DIRECTX
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(ShaderFile, NULL, NULL, ShaderEntry, ShaderVersion,
		dwShaderFlags, 0, NULL, &mptr_ShaderData, &pErrorBlob, NULL);

	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*) pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return false;
	}
	if (pErrorBlob) pErrorBlob->Release();

#endif // USING_DIRECTX

	return true;
}

#ifdef USING_DIRECTX
ID3D11VertexShader * CVertexShader::GetVertexShader()
{
	return mptr_vertexShader;
}

ID3D11VertexShader ** CVertexShader::GetVertexShaderRef()
{
	return &mptr_vertexShader;
}

ID3DBlob * CVertexShader::GetVertexShaderData()
{
	return 	 mptr_ShaderData;
}

ID3DBlob ** CVertexShader::GetVertexShaderDataRef()
{
	return &mptr_ShaderData;
}

#endif // USING_DIRECTX
