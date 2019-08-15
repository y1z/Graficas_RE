#include "CMesh.h"
#include "CDevice.h"
#include "CDeviaceContext.h"
#include <cassert>


CMesh::CMesh(const std::vector<WORD> &Indeces, const std::vector<VertexPosNormTex> &Vertexes, CDevice &Deviace)
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

CMesh::CMesh(const std::vector<unsigned short>& Indeces, const std::vector<VertexPosTex>& Vertexes, CDevice& Deviace)
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

void CMesh::PrepareForDarwCall(CDeviceContext & DeviaceContext)
{
#ifdef USING_DIRECTX

#elif USING_OPEN_GL

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBuffer.GetBufferID());
	unsigned int OffSetOfFirst = offsetof(VertexPosNormTex, Pos);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormTex), reinterpret_cast<const void*>(OffSetOfFirst));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBuffer.GetBufferID());
	unsigned int OffSetOfSecond = offsetof(VertexPosNormTex, Norm);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormTex), reinterpret_cast<const void *>(OffSetOfSecond));

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBuffer.GetBufferID());
	unsigned int OffSetOfThird = offsetof(VertexPosNormTex, TexCoord);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormTex), reinterpret_cast<const void *>(OffSetOfThird));

#endif

}
