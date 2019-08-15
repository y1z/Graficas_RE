#include "CShaderResourceView.h"
#include "CDevice.h"
#include "CDeviaceContext.h"
#include "Utility/ErrorHandlingGrafics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"  



CShaderResourceView::CShaderResourceView()
{}


CShaderResourceView::~CShaderResourceView()
{
#if USING_DIRECTX
	if (mptr_ResourceView != nullptr) { mptr_ResourceView->Release(); }
#else 
	if (m_ResourceID != 0) { glDeleteTextures(1, &m_ResourceID); }
#endif
}


#if USING_DIRECTX
ID3D11ShaderResourceView * CShaderResourceView::GetResourceView()
{
	return mptr_ResourceView;
}

ID3D11ShaderResourceView ** CShaderResourceView::GetResourceViewRef()
{
	return &mptr_ResourceView;
}
#endif // USING_DIRECTX


bool CShaderResourceView::CreateShaderResourceViewFromFile(CDevice &Device, CDeviceContext &DContext, wchar_t *FileName)
{
#ifdef USING_DIRECTX
	HRESULT hr = S_FALSE;

	std::wstring Temp(FileName);


	hr = D3DX11CreateShaderResourceViewFromFile(Device.GetDevice(), FileName, NULL, NULL, &this->mptr_ResourceView, NULL);
	if (!CheckForError(hr))
	{
		return true;
	}
	// check to see if it's an image file 

	if (!CheckForError(hr))
	{
		return true;
	}
	return false;
#endif // USING_DIRECTX



	return false;
}

bool CShaderResourceView::CreateShaderResourceViewFromFile(CDevice & Device, CDeviceContext & DContext, const char * FileName)
{
#ifdef USING_OPEN_GL
	GlRemoveAllErrors();
	int32_t Witdth = 0;
	int32_t Height = 0;
	int32_t ChannalCount = 0;

	mpr_Resouce = stbi_load(FileName, &Witdth, &Height, &ChannalCount, 4);

	ChannalCount = 4;

	glGenTextures(1, &m_ResourceID);
	glBindTexture(GL_TEXTURE_2D, m_ResourceID);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float color[] = { 0.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Witdth, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mpr_Resouce);

	glBindTexture(GL_TEXTURE_2D, 0);

	if(!GlCheckForError())
	{
		return true;
	}
#endif // USING_OPEN_GL
	return false;
}

#ifdef USING_OPEN_GL
uint32_t CShaderResourceView::GetResuorce() const
{
	return m_ResourceID;
}

#endif // DEBUG
