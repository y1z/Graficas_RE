#pragma once

#include "Usable_Windows.h"
#if USING_DIRECTX
#include "DirectXHeader.h"  
#elif USING_OPEN_GL
#endif // USING_DIRECTX

#include <cinttypes>

class CBuffer
{
public:
	CBuffer();
	~CBuffer();
public:// functions 

	/*! This function set up the buffer to later become a Vertex buffer*/
	void InitVertexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeOfBuffer);

	/*! This function set up the buffer to later become a index buffer*/
	void InitIndexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeOfBuffer);

	/*! This function set up the buffer to later become a constant buffer.*/
	void InitConstBuffer(const void *DataStruct, uint32_t Offset, uint32_t SizeOfBuffer,unsigned int &Program , uint32_t Index = 0);

	UINT GetStride();
	UINT GetOffset();
	uint64_t GetElementCount();

#ifdef USING_DIRECTX
	ID3D11Buffer* GetBuffer();
	ID3D11Buffer** GetBufferRef();
	D3D11_BUFFER_DESC GetDesc();
	D3D11_BUFFER_DESC* GetDescRef();
	D3D11_SUBRESOURCE_DATA GetData();
	D3D11_SUBRESOURCE_DATA* GetDataRef();
#elif USING_OPEN_GL

#endif // USING_DIRECT

#if USING_DIRECTX
	D3D11_BUFFER_DESC m_Discriptor;
	D3D11_SUBRESOURCE_DATA m_Data;
private://variables
	ID3D11Buffer *mptr_Buffer = nullptr;

#elif	USING_OPEN_GL	
	uint32_t m_BufferID = 0;
#endif // USING_DIRECTX

	uint32_t m_Stride = 0;
	uint32_t m_Offset = 0;
	uint64_t m_CountElemets = 0;
};