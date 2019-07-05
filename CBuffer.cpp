#include "CBuffer.h"
#if USING_OPEN_GL
#include "OpenglHeader.h"
#endif // USING_OPEN_GL

CBuffer::CBuffer()
{
#if USING_DIRECTX
	memset(&m_Data, 0, sizeof(m_Data));
	memset(&m_Discriptor, 0, sizeof(m_Discriptor));
#endif // USING_DIRECTX

}

CBuffer::~CBuffer()
{
#if USING_DIRECTX
	if (mptr_Buffer != nullptr) { mptr_Buffer->Release(); }
#elif USING_OPEN_GL
	if (m_BufferID != 0) { glDeleteBuffers(1, &m_BufferID); }
#endif // USING_DIRECTX
}

void CBuffer::InitVertexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeSingleElement)
{
#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	m_Discriptor.ByteWidth = SizeSingleElement * TotalElements;
	m_Discriptor.CPUAccessFlags = 0;

	m_CountElemets = TotalElements;

	m_Stride = SizeSingleElement;
	m_Offset = OffSet;

	m_Data.pSysMem = DataStruct;
#elif USING_OPEN_GL

	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ARRAY_BUFFER, SizeOfBuffer, DataStruct, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#endif
}

void CBuffer::InitIndexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeSingleElement)
{
#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;

	m_Discriptor.ByteWidth = SizeSingleElement * TotalElements;
	m_Discriptor.CPUAccessFlags = 0;

	m_CountElemets = TotalElements;

	m_Stride = SizeSingleElement;
	m_Offset = OffSet;

	m_Data.pSysMem = DataStruct;
#elif USING_OPEN_GL
#endif
}

void CBuffer::InitConstBuffer(const void * DataStruct, uint32_t Offset, uint32_t SizeSingleElement)
{
#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.ByteWidth = SizeSingleElement;

	m_Stride = SizeSingleElement;
	m_Offset = Offset;

	m_Discriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Discriptor.CPUAccessFlags = 0;

	m_Data.pSysMem = DataStruct;

#	elif USING_OPEN_GL
#endif
}

UINT CBuffer::GetStride()
{
	return this->m_Stride;
}

UINT CBuffer::GetOffset()
{
	return m_Offset;
}
#if USING_DIRECTX

ID3D11Buffer * CBuffer::GetBuffer()
{
	return this->mptr_Buffer;
}

ID3D11Buffer ** CBuffer::GetBufferRef()
{
	return &mptr_Buffer;
}

D3D11_BUFFER_DESC CBuffer::GetDesc()
{
	return m_Discriptor;
}

D3D11_BUFFER_DESC * CBuffer::GetDescRef()
{
	return &m_Discriptor;
}

D3D11_SUBRESOURCE_DATA CBuffer::GetData()
{
	return m_Data;
}

D3D11_SUBRESOURCE_DATA * CBuffer::GetDataRef()
{
	return &m_Data;
}
#elif USING_OPEN_GL


#endif // USING_DIRECTX

uint64_t CBuffer::GetElementCount()
{
	return m_CountElemets;
}
