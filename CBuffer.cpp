#include "CBuffer.h"
#include <glm/gtc/type_ptr.hpp>
#include "Utility/ErrorHandlingGrafics.h"

CBuffer::CBuffer()
{
	m_Type = BufferType::UNKNOWN;
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
	m_Type = BufferType::Vertex;

#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_Type = static_cast<BufferType>(D3D11_BIND_VERTEX_BUFFER);

	m_Discriptor.ByteWidth = SizeSingleElement * TotalElements;
	m_Discriptor.CPUAccessFlags = 0;

	m_Data.pSysMem = DataStruct;
#elif USING_OPEN_GL
	glGenBuffers(1, &m_BufferID);

	// start using the buffer
	mptr_DataStruct = DataStruct;


	if (GlCheckForError())
	{
		assert(true == false);
		return;
	}

#endif
}

void CBuffer::InitIndexBuffer(const void * DataStruct, uint64_t TotalElements, uint32_t OffSet, uint32_t SizeSingleElement)
{
	m_Stride = SizeSingleElement;
	m_CountElemets = TotalElements;
	m_Offset = OffSet;
	m_Type = BufferType::Index;

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
	mptr_DataStruct = DataStruct;

	GlRemoveAllErrors();
	glGenBuffers(1, &m_BufferID);

	if (GlCheckForError())
	{
		assert(true == false);
	}
#endif
}

void CBuffer::InitConstBuffer(const void *DataStruct, uint32_t Offset, uint32_t SizeOfBuffer, unsigned int &Program, uint32_t Index)
{
	m_Stride = SizeOfBuffer;
	m_Type = BufferType::ConstBuffer;

#if defined(USING_DIRECTX)
	m_Discriptor.Usage = D3D11_USAGE_DEFAULT;
	m_Discriptor.ByteWidth = SizeOfBuffer;

	m_Stride = SizeOfBuffer;
	m_Offset = Offset;

	m_Discriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Discriptor.CPUAccessFlags = 0;

	m_Data.pSysMem = DataStruct;

#	elif USING_OPEN_GL
	mptr_DataStruct = DataStruct;
	GlRemoveAllErrors();

	GLint Location = 0;
	int ActiveUniformCount;
	glGetProgramiv(Program, GL_ACTIVE_UNIFORMS, &ActiveUniformCount);
	glUseProgram(Program);

	if (Index == 0)
	{
		glm::mat4 *Temp = (glm::mat4*) DataStruct;

		m_BufferID = glGetUniformLocation(Program, "u_View");
		glUniformMatrix4fv(m_BufferID, 1, GL_FALSE, (GLfloat*) DataStruct);
	}
	else if (Index == 1)
	{
		m_BufferID = glGetUniformLocation(Program, "u_Projection");
		glUniformMatrix4fv(m_BufferID, 1, GL_FALSE, (GLfloat*) DataStruct);
	}

	else if (Index == 2)
	{
		GlChangesEveryFrameBuf* Temp = (GlChangesEveryFrameBuf*) DataStruct;
		m_BufferID = glGetUniformLocation(Program, "u_World.World");
		glUniformMatrix4fv(m_BufferID, 1, GL_FALSE, &Temp->WorldMatrix[0][0]);

		m_SecondID = glGetUniformLocation(Program, "u_World.LightPos");
		glUniform4fv(m_SecondID, 1, glm::value_ptr(Temp->Color));
	}
	//
	if (GlCheckForError())
	{
		assert(true == false && "Error with Const buffer ");
	}

#endif
}

#ifdef USING_OPEN_GL
void CBuffer::InitConstBuffer(glm::mat4x4 & Matrice, uint32_t Offset, uint32_t SizeOfBuffer, unsigned int & Program, uint32_t Index)
{
	GlRemoveAllErrors();
	m_Stride = SizeOfBuffer;
	m_Type = BufferType::ConstBuffer;
	mptr_DataStruct = static_cast<void*>(&Matrice);

	int ActiveUniformCount;
	glGetProgramiv(Program, GL_ACTIVE_UNIFORMS, &ActiveUniformCount);
	glUseProgram(Program);
	std::string ActiveUniformCountStr = "\nACTIVE UNIFORM COUNT : ";
	ActiveUniformCountStr += std::to_string(ActiveUniformCount);

	OutputDebugStringA(ActiveUniformCountStr.c_str());


	if (Index == 0)
	{
		m_BufferID = glGetUniformLocation(Program, "u_View");
		glUniformMatrix4fv(m_BufferID, 1, GL_FALSE, &Matrice[0][0]);
	}
	else if (Index == 1)
	{
		m_BufferID = glGetUniformLocation(Program, "u_Projection");
		glUniformMatrix4fv(m_BufferID, 1, GL_FALSE, &Matrice[0][0]);
	}

	if (GlCheckForError())
	{
		assert(true == false && "Error with Const buffer ");
	}
}


#endif
UINT CBuffer::GetStride()
{
	return this->m_Stride;
}

UINT CBuffer::GetOffset()
{
	return m_Offset;
}

BufferType CBuffer::GetBufferType() const
{
	return this->m_Type;
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

uint32_t CBuffer::GetBufferID() const
{
	return m_BufferID;
}
#endif // USING_DIRECTX

uint64_t CBuffer::GetElementCount() const
{
	return m_CountElemets;
}
