#include "CCamera.h"
#include "glm/gtc/matrix_transform.hpp"

// creating defualt values 
CCamera::CCamera()
	:m_Far(500.0f),
	m_Near(0.01f),
	// This is 45 degrees in radians
	m_Fov(0.785398163f)
{}

CCamera::~CCamera()
{}

void CCamera::InitCamara(uint32_t WindowWidth, uint32_t WindowHeight)
{
#if defined(USING_DIRECTX)
	m_UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	m_At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	CoordinateUpdate();
	m_Proyection = XMMatrixPerspectiveFovLH(m_Fov, WindowWidth / (FLOAT) WindowHeight, m_Near, m_Far);

	m_View = XMMatrixLookAtLH(m_Eye, m_At, m_UpVector);

	m_Trasfrom = XMMatrixIdentity();
#elif USING_OPEN_GL
	m_UpVector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	m_Eye = glm::vec4(0.0f, 3.0f, -6.0f, 0.0f);
	m_At = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	m_Proyection = glm::perspectiveFovLH(m_Fov, static_cast<float>(WindowWidth),
		static_cast<float>(WindowHeight), m_Near, m_Far);

	m_Trasfrom = glm::identity<glm::mat4x4>();
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
	m_Fov = FovDegrees * (3.14159f / 180.0f);
}

void CCamera::AlterProyectionMatric(int NewWidth, int NewHeight, float Fov, float Near, float Far)
{
	if (Fov < 1.0f)
	{
		Fov = 45.0f;
		SetFov(Fov);
	}
	if (Near < 0.01f)
	{
		Near = 0.01f;
	}
	if (Far < 0.9f)
	{
		Far = 100.0f;
	}
	if (NewWidth < 1)
	{
		NewWidth = 100;
	}
	if (NewHeight < 1)
	{
		NewHeight = 100;
	}
	m_Near = Near;
	m_Far = Far;
#if defined(USING_DIRECTX)

	m_Proyection = XMMatrixPerspectiveFovLH(m_Fov, NewWidth / (float) NewHeight, m_Near, m_Far);
#elif USING_OPEN_GL
	m_Proyection = glm::perspectiveFovLH(m_Fov, static_cast<float>(NewWidth),
		static_cast<float>(NewHeight), m_Near, m_Far);
#endif
}

glm::vec4 CCamera::GetAt() const
{
#ifdef USING_DIRECTX
	glm::vec4 Result;
	Result.x = XMVectorGetByIndex(m_At, 0);
	Result.y = XMVectorGetByIndex(m_At, 1);
	Result.z = XMVectorGetByIndex(m_At, 2);
	Result.w = (1.0f);
	return Result;
#else
	return m_At;
#endif // USING_DIRECTX

}

glm::vec3 CCamera::GetFront() const
{
#ifdef USING_DIRECTX
	glm::vec3 Result;
	Result.x = XMVectorGetByIndex(m_FrontVector, 0);
	Result.y = XMVectorGetByIndex(m_FrontVector, 1);
	Result.z = XMVectorGetByIndex(m_FrontVector, 2);
	return Result;
#else
	return m_FrontVector;
#endif // USING_DIRECTX
}

glm::vec4 CCamera::GetEye() const
{
#ifdef USING_DIRECTX
	glm::vec3 Result;
	Result.x = XMVectorGetByIndex(m_Eye, 0);
	Result.y = XMVectorGetByIndex(m_Eye, 1);
	Result.z = XMVectorGetByIndex(m_Eye, 2);
	return glm::vec4(Result,1.0f);
#else
	return m_Eye;
#endif // USING_DIRECTX

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
	m_Eye += Vec;
	m_At += Vec;
	CoordinateUpdate();
}

#elif USING_OPEN_GL
glm::mat4x4 CCamera::GetViewMatrice()
{
	return m_View;
}
glm::mat4 CCamera::GetTrasformMatrice()
{
	return m_Trasfrom;
}
glm::mat4 CCamera::GetProyectionMatrice()
{
	return m_Proyection;
}

void CCamera::MoveCamera(glm::vec4 arg)
{
	m_At += arg;
	m_Eye += arg;
	this->CoordinateUpdate();
}

#endif // USING_DIRECTX

void CCamera::MoveTrasfromMatrice(float x, float y, float z)
{
#if defined(USING_DIRECTX)
	FXMVECTOR Vector = XMVectorSet(x, y, z, 1);

	m_Trasfrom *= XMMatrixTranslationFromVector(Vector);
#elif USING_OPEN_GL

	glm::translate(m_Trasfrom, glm::vec3(x, y, z));
#endif
}

// TODO : finish implementation
void CCamera::RotateTrasformMatrice(void * Rotation)
{
#if defined(USING_DIRECTX)
	XMVECTOR* Temp = static_cast<XMVECTOR*>(Rotation);
	m_Trasfrom = XMMatrixRotationRollPitchYawFromVector(*Temp);
#elif USING_OPEN_GL
	glm::vec3 *Temp = static_cast<glm::vec3*>(Rotation);
	//glm::rotate()
#endif
}

void CCamera::ResetTrasformMatrice()
{
#if defined(USING_DIRECTX)
	m_Trasfrom = XMMatrixIdentity();
#elif USING_OPEN_GL
	m_Trasfrom = glm::identity<glm::mat4x4>();
#endif
}

void CCamera::CoordinateUpdate()
{
#if defined(USING_DIRECTX)
	m_View = XMMatrixLookAtLH(m_Eye, m_At, m_UpVector);
	// here is the front vector 
	m_FrontVector = XMVector4Normalize(m_At - m_Eye);

	m_RightVector = XMVector4Normalize(XMVector3Cross(m_UpVector, m_FrontVector));

	m_UpVector = (XMVector3Cross(m_FrontVector, m_RightVector));

	m_At = m_Eye + m_FrontVector;
#elif USING_OPEN_GL
	m_View = glm::lookAtLH((glm::vec3) m_Eye, (glm::vec3)m_At, (glm::vec3)m_UpVector);

	m_FrontVector = glm::normalize(m_At - m_Eye);
	glm::vec3 Temp = glm::cross((glm::vec3)m_UpVector, (glm::vec3)m_FrontVector);

	m_RightVector = glm::normalize(glm::vec4(Temp,1.0f));

	m_At = m_Eye + m_FrontVector;
#endif
}


