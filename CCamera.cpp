#include "CCamera.h"
// creating defualt values 
CCamera::CCamera()
	:m_Far(100.0f),
	m_Near(0.01f),
	// This is 45 degrees in radians
	m_Fov(0.785398163f)
{}

CCamera::~CCamera()
{}

void CCamera::InitCamara(uint32_t WindowWidth, uint32_t WindowHeight)
{
#if defined(USING_DIRECTX)
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_View = XMMatrixLookAtLH(Eye, At, Up);
	// here is the front vector 
	m_FrontVector = At - Eye;
	// will use this to determine right and front vector
	m_UpVector = Up;

	m_RightVector = XMVector3Cross(m_UpVector, m_FrontVector);

	m_Proyection = XMMatrixPerspectiveFovLH(m_Fov, WindowWidth / (FLOAT) WindowHeight, m_Near, m_Far);

	m_Trasfrom = XMMatrixIdentity();
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

void CCamera::AlterProyectionMatric(void * Vector)
{
#if defined(USING_DIRECTX)
	XMVECTOR *Result = static_cast<XMVECTOR*>(Vector);
	XMVector3Transform((*Result), m_Proyection);


	m_Proyection = XMMatrixPerspectiveFovLH(m_Fov,  16/ (FLOAT) 9, m_Near, m_Far);
#elif
#endif
}


#if defined(USING_DIRECTX)

XMMATRIX CCamera::GetViewMatrice()
{
	return m_View;
}

XMMATRIX CCamera::GetTrasformMatrice()
{
	return m_Trasfrom;
}

XMMATRIX CCamera::GetProyectionMatrice()
{
	return m_Proyection;
}

#elif
#endif
void CCamera::AlterTrasfromMatrice(float x, float y, float z)
{
#if defined(USING_DIRECTX)
	FXMVECTOR Vector = XMVectorSet(x, y, z, 1);

m_Trasfrom *=	XMMatrixTranslationFromVector(Vector);
#elif
#endif
}

void CCamera::ResetTrasformMatrice()
{
#if defined(USING_DIRECTX)

	m_Trasfrom = XMMatrixIdentity();
#elif
#endif

}


