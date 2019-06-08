#include "CMesh.h"

CMesh::CMesh()
{}

#ifdef USING_DIRECTX

CMesh::CMesh(std::vector<unsigned short> Indeces, std::vector<SimpleVertex> Vertexes)
{
	mptr_IndexBuffer.InitIndexBuffer(&Indeces.front(), Indeces.size(), 0);
	mptr_VertexBuffer.IntiVertexBuffer(&Vertexes.front(), Vertexes.size(), 0);
}

#endif // USING_DIRECTX


CMesh::~CMesh()
{

}

Templates::CBuffer &CMesh::GetIndexBuffer()
{

	return mptr_IndexBuffer;
}

Templates::CBuffer  &CMesh::GetVertexBuffer()
{
	return mptr_VertexBuffer;
}
