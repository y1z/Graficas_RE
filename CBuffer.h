#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include <cinttypes>



class CBuffer
{
public:
	CBuffer()
	{
	#if USING_DIRECTX
		memset(&m_Data, 0, sizeof(m_Data));
		memset(&m_Discriptor, 0, sizeof(m_Discriptor));
	#endif // USING_DIRECTX

	};

	~CBuffer()
	{
	#if USING_DIRECTX
		mptr_Buffer->Release();
	#endif // USING_DIRECTX

	}
public:// functions 

	/*! This function set up the buffer to later become a Vertex buffer*/
	void IntiVertexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeOfBuffer);

	/*! This function set up the buffer to later become a index buffer*/
	void InitIndexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeOfBuffer);

	/*! This function set up the buffer to later become a constant buffer.*/
	void InitConstBuffer(const void *DataStruct, uint32_t Offset, uint32_t SizeOfBuffer);

	UINT GetStride();
	UINT GetOffset();

#ifdef USING_DIRECTX
	ID3D11Buffer* GetBuffer();
	ID3D11Buffer** GetBufferRef();
	D3D11_BUFFER_DESC GetDesc();
	D3D11_BUFFER_DESC* GetDescRef();
	D3D11_SUBRESOURCE_DATA GetData();
	D3D11_SUBRESOURCE_DATA* GetDataRef();
#endif // USING_DIRECT

	uint64_t GetElementCount();
	// 
#if USING_DIRECTX

	D3D11_BUFFER_DESC m_Discriptor;
	D3D11_SUBRESOURCE_DATA m_Data;

private://variables
	ID3D11Buffer *mptr_Buffer = nullptr;

#endif // USING_DIRECTX

	uint32_t m_Stride = 0;
	uint32_t m_Offset = 0;
	uint64_t m_CountElemets = 0;
};