#pragma once
#include "DirectXHeader.h"

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
#ifdef USING_DIRECTX

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct CBNeverChanges
{
	XMMATRIX mView;
};

struct CBChangeOnResize
{
	XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
	XMMATRIX mWorld;
	XMFLOAT4 vMeshColor;
};
//  
#endif // USING_DIRECTX
