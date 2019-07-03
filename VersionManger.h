#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
/*!
\brief this takes care of telling find-out which version of whatever 
 Graphics api I'm using and returning the appropriate value/tag to inform
*/

class VersionManger
{
public:
	VersionManger();
	~VersionManger();




#ifdef USING_DIRECTX


#else 

#endif // USING_DIRECTX



private:
#if USING_DIRECTX
	D3D_DRIVER_TYPE m_DxDirver;
	D3D_DRIVER_TYPE m_DxFeature;
#endif // USING_DIRECTX


};

