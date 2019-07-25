#pragma once
#include "Usable_Windows.h"
#include "GraphicsLIbsHeader.h"

/*!Heres a descriptor struct*/
struct ViewPortDesc
{
	float Height;
	float Width;
	float TopLeftX;
	float TopLeftY;
};
/*! here is the class*/
class CViewPort
{
public:
	CViewPort();
	~CViewPort();
	/*height*/
	void SetupViewPort(float height,float Width, float TopRightPosX,float TopLeftPosY);

	void IncreamentWidth();
	void DecreamentWidth();
	void IncreamentHieght();

#ifdef USING_DIRECTX
	D3D11_VIEWPORT GetViewPort();
	D3D11_VIEWPORT *GetViewPortRef();
#else
#endif // USING_DIRECTX


private:
	ViewPortDesc m_ViewPort;
#if defined(USING_DIRECTX)
	//use this when working with directX 
	D3D11_VIEWPORT m_dxViewport;
#else 
#endif

};
