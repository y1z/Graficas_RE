#pragma once
#include "GraphicsLIbsHeader.h"
#include "Custom_Structs.h"
#include <vector>
#include "CBuffer.h"

class CDeviceContext;
class CDevice;

class CMesh
{
public:
	/*! Constructor for when I have  Position, Normal and Texcoord*/
	CMesh(const std::vector<unsigned short> &Indeces, const std::vector<VertexPosNormTex> &Vertexes, CDevice &Deviace);

	CMesh(const std::vector<unsigned short> &Indeces, const std::vector<VertexPosTex> &Vertexes, CDevice &Deviace);

	~CMesh();

public:
	/// MUST BE REFERECES or temporary s will be created and destroyed freeing memory 
	/// thats still in use 
	//! retuns a reference to the index buffer 
	CBuffer &GetIndexBuffer();
	//! returns a reference to the vertex buffer 
	CBuffer &GetVertexBuffer();

	void PrepareForDarwCall(CDeviceContext &DeviaceContext);
private:
	CBuffer m_IndexBuffer;
	CBuffer m_VertexBuffer;

};

