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
	XMVECTOR  Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	 m_Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	  m_At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_View = XMMatrixLookAtLH(m_Eye, m_At, Up);
	// here is the front vector 
	m_FrontVector = m_At - m_Eye;
	
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

void CCamera::AlterProyectionMatric(int NewWidth, int NewHeight, float Fov , float Near , float Far)
{
#if defined(USING_DIRECTX)
	if (Fov < 1)
	{
		Fov = 45.0f;
	}
	if (Near < 0.01f)
	{
		Near = 0.01f;
	}
	if (Far < 0.9f)
	{
		Far = 100.0f;
	}
	m_Fov = Fov;
	m_Near = Near;
	m_Far = Far;


	m_Proyection = XMMatrixPerspectiveFovLH(m_Fov, NewWidth / (FLOAT) NewHeight, m_Near, m_Far);
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

void CCamera::MoveCamera(XMVECTOR Vec)
{

	m_Trasfrom *= XMMatrixTranslationFromVector(Vec);
}

#elif
#endif
void CCamera::MoveTrasfromMatrice(float x, float y, float z)
{
#if defined(USING_DIRECTX)
	FXMVECTOR Vector = XMVectorSet(x, y, z, 1);

	m_Trasfrom *= XMMatrixTranslationFromVector(Vector);
#elif
#endif
}

void CCamera::RotateTrasformMatrice(void * Rotation)
{
#if defined(USING_DIRECTX)
	XMMATRIX* Temp = static_cast<XMMATRIX*>(Rotation);
	m_Trasfrom *= (*Temp);
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

void CCamera::CoordinateUpdate()
{
#if defined(USING_DIRECTX)
	m_View = XMMatrixLookAtLH(m_Eye, m_At, m_UpVector	);
	// here is the front vector 
	m_FrontVector = m_At - m_Eye;
	m_RightVector = XMVector3Cross(m_UpVector, m_FrontVector);
#elif
#endif
}


