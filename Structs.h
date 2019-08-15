#pragma once
#include "GraphicsLIbsHeader.h"

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
#ifdef USING_DIRECTX

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct CBNeverChanges_View
{
	XMMATRIX mView;
	// where the camera is looking at 
	glm::vec3 AtVector;
};

struct CBChangeOnResize_Projection
{
	XMMATRIX mProjection;
};

struct CBChangesEveryFrame_World
{
	XMMATRIX mWorld;
	XMFLOAT4 vMeshColor;
};
//  
#endif // USING_DIRECTX
