#include "ViewPort.h"

CViewPort::CViewPort()
{}

CViewPort::~CViewPort()
{}



void CViewPort::SetupViewPort(float height, float Width, float TopLeftPosX, float TopLeftPosY)
{
#ifdef USING_DIRECTX
	m_dxViewport.Height = height;
	m_dxViewport.Width = Width;
	m_dxViewport.TopLeftX = TopLeftPosX;
	m_dxViewport.TopLeftY = TopLeftPosY;
	m_dxViewport.MinDepth = 0.0f;
	m_dxViewport.MaxDepth = 1.0f;
#else// TODO_GL 	
	glViewport(0, 0, Width, height);

#endif // USING_DIRECTX
}

void CViewPort::IncreamentWidth()
{
#ifdef USING_DIRECTX
	m_dxViewport.Width += 10.0f;
#else
#endif // USING_DIRECTX

}

void CViewPort::DecreamentWidth()
{
#ifdef USING_DIRECTX
	m_dxViewport.Width -= 10.0f;
#else
#endif // USING_DIRECTX
}

void CViewPort::IncreamentHieght()
{
#ifdef USING_DIRECTX
	m_dxViewport.Height += 1.0f;
#else
#endif // USING_DIRECTX

}

#ifdef USING_DIRECTX
D3D11_VIEWPORT CViewPort::GetViewPort()
{
	return m_dxViewport;
}
D3D11_VIEWPORT * CViewPort::GetViewPortRef()
{
	return &m_dxViewport;
}
#endif // DEBUG