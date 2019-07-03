#include "CMesh.h"

CMesh::CMesh()
{}

#ifdef USING_DIRECTX

CMesh::CMesh(std::vector<unsigned short> Indeces, std::vector<SimpleVertex> Vertexes)
{
	mptr_IndexBuffer.InitIndexBuffer(&Indeces.front(), Indeces.size(), 0,sizeof(unsigned short));
	mptr_VertexBuffer.IntiVertexBuffer(&Vertexes.front(), Vertexes.size(), 0, sizeof(SimpleVertex));
}

#endif // USING_DIRECTX


CMesh::~CMesh()
{

}

CBuffer &CMesh::GetIndexBuffer()
{

	return mptr_IndexBuffer;
}

CBuffer  &CMesh::GetVertexBuffer()
{
	return mptr_VertexBuffer;
}
