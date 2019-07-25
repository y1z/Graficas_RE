#pragma once

#include "GraphicsLIbsHeader.h"
#include <cinttypes>


enum class BufferType
{
	UNKNOWN,
#if USING_DIRECTX

	Vertex = D3D11_BIND_VERTEX_BUFFER,
	Index = D3D11_BIND_INDEX_BUFFER,
	ConstBuffer = D3D11_BIND_CONSTANT_BUFFER

#elif USING_OPEN_GL
	Vertex = 1,//<! where the geometry is 
	Index = 2,//<! which order to draw the vertices 
	ConstBuffer = 3//<! buffer that does not change during the render call 
#endif // USING_DIRECTX

};


class CBuffer
{
public:
	CBuffer();
	~CBuffer();
public:// functions 

	/*! This function set up the buffer to later become a Vertex buffer*/
	void InitVertexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeSingleElement);

	/*! This function set up the buffer to later become a index buffer*/
	void InitIndexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeSingleElement);

	/*! This function set up the buffer to later become a constant buffer.*/
	void InitConstBuffer(const void *DataStruct, uint32_t Offset, uint32_t SizeOfBuffer, unsigned int &Program, uint32_t Index = 0);

	UINT GetStride();
	UINT GetOffset();

	[[nodiscard]]
	BufferType GetBufferType() const;

	uint64_t GetElementCount() const;

#ifdef USING_DIRECTX
	ID3D11Buffer* GetBuffer();
	ID3D11Buffer** GetBufferRef();
	D3D11_BUFFER_DESC GetDesc();
	D3D11_BUFFER_DESC* GetDescRef();
	D3D11_SUBRESOURCE_DATA GetData();
	D3D11_SUBRESOURCE_DATA* GetDataRef();
#elif USING_OPEN_GL
	uint32_t GetBufferID() const;
#endif // USING_DIRECT

#if USING_DIRECTX
	D3D11_BUFFER_DESC m_Discriptor;
	D3D11_SUBRESOURCE_DATA m_Data;
private://variables
	ID3D11Buffer *mptr_Buffer = nullptr;

#elif	USING_OPEN_GL	
	uint32_t m_BufferID = 0;

	const void * mptr_DataStruct = nullptr;
#endif // USING_DIRECTX
	//-----	API independent variables	-
	//! how big a single element(struct) of a buffer is in bytes 
	uint32_t m_Stride = 0;
	uint32_t m_Offset = 0;
	//! how many element the buffer contains
	uint64_t m_CountElemets = 0;
	//! what type of buffer this buffer is 
	BufferType m_Type;
};