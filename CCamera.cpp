#include "CCamera.h"
// creating defualt values 
CCamera::CCamera()
	:m_Far(100.0f),
	m_Near(0.01f),
	// This is 45 degrees in radians
	m_Fov(0.785398163f)
{
}

CCamera::~CCamera()
{
}

void CCamera::InitCamara(uint32_t WindowWidth, uint32_t WindowHeight)
{
#if defined(USING_DIRECTX)
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_View = XMMatrixLookAtLH(Eye, At, Up);

	m_Proyection = XMMatrixPerspectiveFovLH(m_Fov, WindowWidth / (FLOAT)WindowHeight, m_Near, m_Far);
	// will use this to determine right and front vector
	m_UpVector = Up;
	// using this because the coordinate system is already using the 
	// left hand rule http://www.f-lohmueller.de/pov_tut/a_geo/a_geo85e.htm
	m_RightVector = XMVectorSet(1.0f, 0, 0, 0);

	/*if put in the reverse order the front vector would be negative*/
	m_FrontVector = XMVector3Cross(m_RightVector, m_UpVector);

#elif
#endif
}

void CCamera::SetFar(float newFarValue)
{
	this->m_Far = newFarValue;
}

void CCamera::SetNear(float newNearValue)
{
	this->m_Near = newNearValue;
}

void CCamera::SetFov(float FovDegrees)
{
	m_Fov = FovDegrees * (3.14159f / 180);
}


#if defined(USING_DIRECTX)

XMMATRIX CCamera::GetViewMatrice()
{
	return m_View;
}

XMMATRIX CCamera::GetProyectionMatrice()
{
	return m_Proyection;
}
#elif
#endif


