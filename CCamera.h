#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include <cinttypes>
#include <cmath>

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

#if defined(USING_DIRECTX)
	XMMATRIX GetViewMatrice();
	XMMATRIX GetTrasformMatrice();
	XMMATRIX GetProyectionMatrice();
#elif
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

	XMVECTOR m_Up;
	XMVECTOR m_Eye;
	XMVECTOR m_At;


#elif
#endif
};

