#pragma once
#include "GraphicsLIbsHeader.h"
#include <glm/glm.hpp>
#include <cinttypes>

class CCamera
{
public:// constructors
	CCamera();
	~CCamera();
public:// functions 
	/*! prepares the camera for creation*/
	void InitCamara(uint32_t WindowWidth, uint32_t WindowHieght);
	/*! this in the case we want to change the maximum view distance*/
	void SetFar(float newFarValue);
	/*! this for when we want to change the minimum view distance*/
	void SetNear(float newNearValue);
	/*! Set FOV */
	void SetFov(float FovDegrees);
	/*! this methods lets my multiply the matrices by a vector */
	void AlterProyectionMatric(int NewWidth, int NewHeight, float Fov = -1, float Near = -1, float Far = -1);

	void MoveTrasfromMatrice(float x, float y, float z);
	
	void RotateTrasformMatrice(void * Rotation);

	void ResetTrasformMatrice();

	void CoordinateUpdate();
	//! returns the member m_At
	glm::vec4 GetAt() const;

	glm::vec3 GetFront() const;

	glm::vec4 GetEye() const;

#if defined(USING_DIRECTX)
	XMMATRIX GetViewMatrice();
	XMMATRIX GetTrasformMatrice();
	XMMATRIX GetProyectionMatrice();

	void MoveCamera(XMVECTOR Vec);
#else

	glm::mat4x4 GetViewMatrice();
	glm::mat4 GetTrasformMatrice();
	glm::mat4 GetProyectionMatrice();
	
	void MoveCamera(glm::vec4 arg);
#endif

private:
	// api independent values 
	bool isLeftHandRule = true;
	/*! the maximum distance of the camera*/
	float m_Far;
	/*! the minimum distance of the camera*/
	float m_Near;
	/*! determines the fov */
	float m_Fov;

#if defined(USING_DIRECTX)
public:// REMOVE public 
	// matrices
	XMMATRIX m_Trasfrom;
	XMMATRIX m_View;
	XMMATRIX m_Proyection;
	// Position vectors
	XMVECTOR m_RightVector;
	XMVECTOR m_UpVector;
	XMVECTOR m_FrontVector;
	//! used to know where the camera is located 
	XMVECTOR m_Eye;
	//! used to know where the camera is looking at 
	XMVECTOR m_At;

#else
	// matrices
	glm::mat4x4 m_Trasfrom;
	glm::mat4x4 m_View;
	glm::mat4x4 m_Proyection;
	// Position vectors 
	glm::vec4 m_RightVector;
	glm::vec4 m_UpVector;
	glm::vec4 m_FrontVector;
	//! used to know where the camera is located 
	glm::vec4 m_Eye;
	//! used to know where the camera is looking at 
	glm::vec4 m_At;
#endif
};

