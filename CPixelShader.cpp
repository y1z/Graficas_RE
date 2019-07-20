#include "CPixelShader.h"
#include "Utility/ErrorHandlingGrafics.h"
#include "Utility/FileHelper.h"

CPixelShader::CPixelShader()
{}


CPixelShader::~CPixelShader()
{
#if USING_DIRECTX
	if (mptr_PixelShader) { mptr_PixelShader->Release(); }
	if (mptr_ShaderData) { mptr_ShaderData->Release(); }
#endif // USING_DIRECTX

}

bool CPixelShader::InitPixelShader(wchar_t * ShaderFile, char * ShaderEntry, char * ShaderVersion)
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
#elif USING_OPEN_GL

#endif // USING_DIRECTX

	return true;
}

bool CPixelShader::InitPixelShader(const char * ShaderFile)
{
#if USING_OPEN_GL
	GlRemoveAllErrors();
	m_Shader = FileHelper::ReadFileGl(ShaderFile, 2);
	m_PixelShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (!GlCheckForError())
	{
		return true;
	}

	return false;
#endif // USING_OPEN_GL
	return false;
}

#if USING_DIRECTX
ID3D11PixelShader * CPixelShader::GetPixelShader()
{
	return mptr_PixelShader;
}

ID3D11PixelShader ** CPixelShader::GetPixelShaderRef()
{
	return &mptr_PixelShader;
}

ID3DBlob * CPixelShader::GetPixelShaderData()
{
	return mptr_ShaderData;
}

ID3DBlob ** CPixelShader::GetPixelShaderDataRef()
{
	return &mptr_ShaderData;
}

#endif // USING_DIRECTX
