#include "CShaderResourceView.h"
#include "CDevice.h"
#include "Utility/ErrorHandlingGrafics.h"

CShaderResourceView::CShaderResourceView()
{}


CShaderResourceView::~CShaderResourceView()
{
#if USING_DIRECTX
	if (mptr_ResourceView != nullptr) { mptr_ResourceView->Release(); }
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


bool CShaderResourceView::CreateShaderResourceViewFromFile(CDevice & Device, wchar_t * FileName)
{
	HRESULT hr = S_FALSE;
#ifdef USING_DIRECTX
	hr =  D3DX11CreateShaderResourceViewFromFile(Device.GetDevice(), FileName, NULL, NULL, &this->mptr_ResourceView, NULL);
	if (!CheckForError(hr))
	{
		return true;
	}
	return false;
#endif // USING_DIRECTX
	
	return false;
}
