#if USING_OPEN_GL
#include "OpenglHeader.h"
#include "Utility/ErrorHandlingGrafics.h"
#include "Custom_Structs.h"
#include "glm.hpp"
#endif // USING_OPEN_GL

#include "CBuffer.h"
CBuffer::CBuffer()
{
#if USING_DIRECTX
	memset(&m_Data, 0, sizeof(m_Data));
	memset(&m_Discriptor, 0, sizeof(m_Discriptor));
#elif USING_OPEN_GL

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
	// api independent variables 
	m_Stride = SizeSingleElement;
	m_Offset = OffSet;
	m_CountElemets = TotalElements;

#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	m_Discriptor.ByteWidth = SizeSingleElement * TotalElements;
	m_Discriptor.CPUAccessFlags = 0;

	m_Data.pSysMem = DataStruct;
#elif USING_OPEN_GL

	glGenBuffers(1, &m_BufferID);
	// start using the buffer
	glEnableVertexAttribArray(0);// pos 
	glEnableVertexAttribArray(1);// texcoord 
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ARRAY_BUFFER, SizeSingleElement, DataStruct, GL_DYNAMIC_DRAW);

	/// THIS is temporary 
	unsigned int OffSetFirstMember = offsetof(VertexWithTexture, Pos);
	unsigned int OffSetSecondMember = offsetof(VertexWithTexture, TexCoord);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithTexture), reinterpret_cast<const void*>(OffSetFirstMember));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithTexture), reinterpret_cast<const void*>(OffSetSecondMember));
	// end using the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (GlCheckForError())
	{
		assert(true == false);
	}

	return;

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
	GlRemoveAllErrors();
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeSingleElement * TotalElements, DataStruct, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (GlCheckForError())
	{
		assert(true == false);
	}
#endif
}

void CBuffer::InitConstBuffer(const void *DataStruct, uint32_t Offset, uint32_t SizeOfBuffer, unsigned int &Program, uint32_t Index)
{
#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.ByteWidth = SizeOfBuffer;

	m_Stride = SizeOfBuffer;
	m_Offset = Offset;

	m_Discriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Discriptor.CPUAccessFlags = 0;

	m_Data.pSysMem = DataStruct;

#	elif USING_OPEN_GL
	GlRemoveAllErrors();
	GLint Location = 0;
	if (Index == 0)
	{
		Location = glGetUniformLocation(Program, "u_NeverChanges");
		glUniformMatrix4fv(Location, 1, GL_FALSE, (GLfloat*) DataStruct);

	}
	else if (Index == 1)
	{
		Location = glGetUniformLocation(Program, "u_ChangeOnRisize");
		glUniformMatrix4fv(Location, 1, GL_FALSE, (GLfloat*) DataStruct);
	}
	else if (Index == 2)
	{
		Location = glGetUniformLocation(Program, "u_ChangeEveryFrame");
		glUniformMatrix4fv(Location, 2, GL_FALSE, (GLfloat*) DataStruct);
	}

	if (GlCheckForError())
	{
		assert(true == false);
	}

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
