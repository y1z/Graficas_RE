#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include "Custom_Structs.h"
#include <vector>
#include "CBuffer.h"

class CDeviceContext;
class CDevice;

class CMesh
{
public:
	CMesh(const std::vector<unsigned short> &Indeces, const std::vector<VertexWithTexture> &Vertexes, CDevice &Deviace);
	~CMesh();

public:
	/// MUST BE REFERECES or temporary s will be created and destroyed freeing memory 
	/// thats still in use 
	CBuffer &GetIndexBuffer();
	CBuffer &GetVertexBuffer();

	void DarwSelf(CDeviceContext &DeviaceContext);
private:
	CBuffer m_IndexBuffer;
	CBuffer m_VertexBuffer;

};

