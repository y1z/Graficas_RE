#include "CShaderResourceView.h"



CShaderResourceView::CShaderResourceView()
{}


CShaderResourceView::~CShaderResourceView()
{
	if (mptr_ResourceView != nullptr) { mptr_ResourceView->Release(); }
}

ID3D11ShaderResourceView * CShaderResourceView::GetResourceView()
{
	return mptr_ResourceView;
}

ID3D11ShaderResourceView ** CShaderResourceView::GetResourceViewRef()
{
	return &mptr_ResourceView;
}
