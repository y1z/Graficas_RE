#include "CBuffer.h"

void CBuffer::IntiVertexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeOfBuffer)
{
#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	m_Discriptor.ByteWidth = SizeOfBuffer * TotalElements;
	m_Discriptor.CPUAccessFlags = 0;

	m_CountElemets = TotalElements;

	m_Stride = SizeOfBuffer;
	m_Offset = OffSet;

	m_Data.pSysMem = DataStruct;
#elif
#endif

}

void CBuffer::InitIndexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeOfBuffer)
{
#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;

	m_Discriptor.ByteWidth = SizeOfBuffer * TotalElements;
	m_Discriptor.CPUAccessFlags = 0;

	m_CountElemets = TotalElements;

	m_Stride = SizeOfBuffer;
	m_Offset = OffSet;

	m_Data.pSysMem = DataStruct;
#elif
#endif
}

void CBuffer::InitConstBuffer(const void * DataStruct, uint32_t Offset, uint32_t SizeOfBuffer)
{
#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.ByteWidth = SizeOfBuffer;

	m_Stride = SizeOfBuffer;
	m_Offset = Offset;

	m_Discriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Discriptor.CPUAccessFlags = 0;

	m_Data.pSysMem = DataStruct;

#	elif
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

uint64_t CBuffer::GetElementCount()
{
	return m_CountElemets;
}
