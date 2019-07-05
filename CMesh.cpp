#include "CMesh.h"
#include "CDevice.h"
#include "CDeviaceContext.h"
#include <cassert>

CMesh::CMesh(const std::vector<WORD> &Indeces, const std::vector<VertexWithTexture> &Vertexes, CDevice &Deviace)
{
	bool isSuccesful = true;
	m_IndexBuffer.InitIndexBuffer(&Indeces.front(), Indeces.size(), 0, sizeof(WORD));
	isSuccesful = Deviace.CreateBuffer(m_IndexBuffer);
	// checking for errors 
	assert(isSuccesful == true);

	m_VertexBuffer.InitVertexBuffer(&Vertexes.front(), Vertexes.size(), 0, sizeof(Vertexes[0]));
	isSuccesful = Deviace.CreateBuffer(m_VertexBuffer);
	// checking for errors 
	assert(isSuccesful == true);
}

CMesh::~CMesh()
{}

CBuffer &CMesh::GetIndexBuffer()
{
	return this->m_IndexBuffer;
}

CBuffer &CMesh::GetVertexBuffer()
{
	return this->m_VertexBuffer;
}

void CMesh::DarwSelf(CDeviceContext & DeviaceContext)
{
#ifdef USING_DIRECTX

#elif USING_OPEN_GL

#endif

}
