#include "CDeviaceContext.h"
#include "CRenderTragetView.h"
#include "CDepthStencilView.h"
#include "ViewPort.h"
#include "CInputLayout.h"
#include "CBuffer.h"
#include "CVertexShader.h"
#include "CPixelShader.h"
#include "CShaderResourceView.h"
#include "CSampler.h"
#include "Utility/ErrorHandlingGrafics.h"
#include "Custom_Structs.h"
#include <cassert>

CDeviceContext::CDeviceContext()
{}


CDeviceContext::~CDeviceContext()
{
#if USING_DIRECTX
	if (mptr_DeviceContext != nullptr) { mptr_DeviceContext->Release(); }
#endif // USING_DIRECTX

}


void CDeviceContext::OMSetRenderTargets(int32_t totalViews, CRenderTragetView &renderTragetView, CDepthStencilView &DepthStencilView)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->OMSetRenderTargets(totalViews,
		static_cast<ID3D11RenderTargetView**>(renderTragetView.GetRenderTragetRef()),
		static_cast<ID3D11DepthStencilView*>(DepthStencilView.GetDepthStencilView()));
#elif USING_OPEN_GL


#endif
}

void CDeviceContext::RSSetViewports(int32_t TotalViews, CViewPort &ViewPort)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->RSSetViewports(TotalViews, static_cast<D3D11_VIEWPORT*>(ViewPort.GetViewPortRef()));
#elif USING_OPEN_GL
#endif

}


void CDeviceContext::IASetInputLayout(CInputLayout &InputLayout)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->IASetInputLayout(static_cast<ID3D11InputLayout*>(InputLayout.GetInputLayout()));
#elif USING_OPEN_GL

#endif

}

void CDeviceContext::IASetVertexBuffers(int32_t StratSlot, int32_t TotalBuffer, CBuffer &VertexBuffer, uint32_t Stride, uint32_t Offset)
{
#if defined(USING_DIRECTX)
	UINT ConversionForStride = Stride;
	UINT ConversionForOffset = Offset;

	mptr_DeviceContext->IASetVertexBuffers(StratSlot, TotalBuffer,
		static_cast<ID3D11Buffer **>(VertexBuffer.GetBufferRef()), static_cast<UINT *>(&ConversionForStride),
		static_cast<UINT *>(&ConversionForOffset));
#elif USING_OPEN_GL
#endif
}

void CDeviceContext::IASetIndexBuffer(CBuffer& IndexBuffer, int Format, int Offset)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->IASetIndexBuffer(static_cast<ID3D11Buffer*>(IndexBuffer.GetBuffer()),
		static_cast<DXGI_FORMAT>(Format), Offset);
#elif USING_OPEN_GL
#endif
}

/*! set the topology
\param Topology [in] dictates how the geometry is going to be drawn */
void CDeviceContext::IASetPrimitiveTopology(int Topology)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(Topology));

#elif USING_OPEN_GL

#endif
}

/*TODO : make many overloads for this function */
void CDeviceContext::UpdateSubresource(CBuffer &DistResource, void* OriginResrouce,
	int32_t IndexUpdateResource, void *ReservedCopyLocation,
	uint32_t PitchRow, uint32_t PitchDepth)
{

#if defined(USING_DIRECTX)
	if (ReservedCopyLocation == nullptr)
	{
		mptr_DeviceContext->UpdateSubresource(static_cast<ID3D11Resource *>(DistResource.GetBuffer()), IndexUpdateResource,
			nullptr, OriginResrouce,
			PitchRow, PitchDepth);
	}
	else
	{
		mptr_DeviceContext->UpdateSubresource(static_cast<ID3D11Resource *>(DistResource.GetBuffer()), IndexUpdateResource,
			static_cast<D3D11_BOX*>(ReservedCopyLocation), OriginResrouce,
			PitchRow, PitchDepth);
	}
#elif USING_OPEN_GL
#endif
}

/*! dictates what color the RenderTraget
\param RenderTraget[out] the render target (ID3D11RenderTargetView*)
\param Color [in] dictates which color the screen will clear to*/
void CDeviceContext::ClearRenderTargetView(CRenderTragetView &RenderTraget, float * Color)
{
#if defined(USING_DIRECTX)
	if (Color == nullptr)
	{
		/*Should be Purple*/
		float Purple[4] = { (255 / 136),(255 / 50),(255 / 249),1.0f };

		mptr_DeviceContext->ClearRenderTargetView(static_cast<ID3D11RenderTargetView*>(RenderTraget.GetRenderTraget()), Purple);
	}
	else
	{
		mptr_DeviceContext->ClearRenderTargetView(static_cast<ID3D11RenderTargetView*>(RenderTraget.GetRenderTraget()), Color);
	}
#elif USING_OPEN_GL
	glClearColor(0.5f, 0, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

}

void CDeviceContext::ClearDepthStencilView(CDepthStencilView &DepthStencilView, int Flags, float DepthBufferClear, int8_t ClearStencil)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->ClearDepthStencilView(static_cast<ID3D11DepthStencilView*>(DepthStencilView.GetDepthStencilView())
		, Flags
		, DepthBufferClear
		, ClearStencil);
#elif USING_OPEN_GL
#endif
}

void CDeviceContext::VSSetShader(CVertexShader &VertexShader)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->VSSetShader(static_cast<ID3D11VertexShader*>(VertexShader.GetVertexShader()), nullptr, 0);
#elif USING_OPEN_GL
#endif
}

void CDeviceContext::VSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, CBuffer *Buffer)
{
#if defined(USING_DIRECTX)
	for (int i = 0; i < TotalBuffers; ++i)
	{
		mptr_DeviceContext->VSSetConstantBuffers(StartSlot, TotalBuffers, static_cast<ID3D11Buffer**>(Buffer[i].GetBufferRef()));

	}
#elif USING_OPEN_GL
	GlRemoveAllErrors();
	if (StartSlot == 0)
	{
		glUniformMatrix4fv(Buffer[0].m_BufferID, 1, GL_FALSE, (GLfloat*) Buffer->mptr_DataStruct);
	}
	else if (StartSlot == 1)
	{
		glUniformMatrix4fv(Buffer[0].m_BufferID, 1, GL_FALSE, (GLfloat*) Buffer->mptr_DataStruct);
	}

	else if (StartSlot == 2)
	{
		GlChangesEveryFrameBuf* Temp = (GlChangesEveryFrameBuf*) Buffer->mptr_DataStruct;
		glUniformMatrix4fv(Buffer[0].m_BufferID, 1, GL_FALSE, &Temp->WorldMatrix[0][0]);

		glUniform4fv(Buffer[0].m_SecondID, 1, glm::value_ptr(Temp->Color));
	}
	if (GlCheckForError())
	{
		assert(true == false, "Error with setting the unifomrs ");
	}

#endif
}

//! OVERLOAD for openGL 
void CDeviceContext::VSSetConstantBuffers(uint8_t Index, CBuffer &Buffer, glm::mat4x4 &Matrix)
{
#ifdef USING_OPEN_GL
	GlRemoveAllErrors();

	if (Index == 0 || Index == 1)
	{
		glUniformMatrix4fv(Buffer.GetBufferID(), 1, GL_FALSE, &Matrix[0][0]);
	}

	//else if (StartSlot == 2)
	//{
	//	GlChangesEveryFrameBuf* Temp = (GlChangesEveryFrameBuf*) Buffer->mptr_DataStruct;
	//	glUniformMatrix4fv(Buffer[0].m_BufferID, 1, GL_FALSE, &Temp->WorldMatrix[0][0]);
	//
	//	glUniform4fv(Buffer[0].m_SecondID, 1, Temp->Color);
	//}
	if (GlCheckForError())
	{
		assert(true == false, "Error with setting the unifomrs ");
	}

#endif // USING_OPEN_GL


}
//! OVERLOAD  for openGLS
void CDeviceContext::VSSetConstantBuffers(uint8_t Index, CBuffer &Buffer, GlChangesEveryFrameBuf &WorldAndColor)
{
#ifdef USING_OPEN_GL
	GlRemoveAllErrors();

	glUniformMatrix4fv(Buffer.GetBufferID(), 1, GL_FALSE, &WorldAndColor.WorldMatrix[0][0]);

	glUniform4fv(Buffer.m_SecondID, 1, glm::value_ptr(WorldAndColor.Color));

	if (GlCheckForError())
	{
		assert(true == false, "Error with setting the unifomrs ");
	}

#endif // USING_OPEN_GLs

}

void CDeviceContext::PSSetShader(CPixelShader &PixelShader)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetShader(static_cast<ID3D11PixelShader*>(PixelShader.GetPixelShader()), nullptr, 0);
#elif USING_OPEN_GL
#endif
}

void CDeviceContext::PSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, CBuffer *Buffer)
{
#if defined(USING_DIRECTX)
	for (int i = 0; i < TotalBuffers; ++i)
	{
		mptr_DeviceContext->PSSetConstantBuffers(StartSlot, TotalBuffers, static_cast<ID3D11Buffer**>(Buffer[i].GetBufferRef()));
	}
#elif USING_OPEN_GL
#endif
}

void CDeviceContext::PSSetShaderResources(int32_t StratSlot, int32_t TotalViews, CShaderResourceView &ShaderResourceView)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetShaderResources(StratSlot, TotalViews, static_cast<ID3D11ShaderResourceView**>(ShaderResourceView.GetResourceViewRef()));
#elif USING_OPEN_GL


#endif
}

void CDeviceContext::PSSetShaderResources(uint32_t & Program, CShaderResourceView & ShaderResourceView)
{
#ifdef USING_OPEN_GL
	GlRemoveAllErrors();
	// should be the index for shader program 
	glUseProgram(Program);

	int Location = glGetUniformLocation(Program, "myTextureSampler");

	glActiveTexture(GL_TEXTURE0 + ShaderResourceView.GetResuorce());

	glBindTexture(GL_TEXTURE_2D, ShaderResourceView.GetResuorce());
	// NEEDS TO BE ONE OR EVERYTHING GOES TO SHIT 
	glUniform1i(Location,1);

	if (GlCheckForError())
	{
		assert(true == false, "Error with Texture Sample");
	}
#endif

}

void CDeviceContext::PSSetSamplers(int32_t StartSlot, int32_t TotalSamplers, CSampler &Sampler)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetSamplers(StartSlot, TotalSamplers, static_cast<ID3D11SamplerState**>(Sampler.GetSamplerRef()));
#elif USING_OPEN_GL
#endif 
}

void CDeviceContext::DrawIndexed(int32_t TotalIndexs, int32_t StartIndex, int32_t IndexDiff)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->DrawIndexed(TotalIndexs, StartIndex, IndexDiff);
#elif USING_OPEN_GL
#endif
}

void CDeviceContext::DrawIndexed(CBuffer & IndexBuffer)
{

#if defined(USING_DIRECTX)
	mptr_DeviceContext->DrawIndexed(IndexBuffer.GetElementCount(), 0, 0);
#elif USING_OPEN_GL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer.GetBufferID());

	glDrawElements(GL_TRIANGLES, IndexBuffer.GetElementCount(), GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#endif
}


#if defined(USING_DIRECTX)

void CDeviceContext::DestroySelf()
{
	mptr_DeviceContext->Release();
	mptr_DeviceContext = nullptr;
}

ID3D11DeviceContext **CDeviceContext::GetDeviceContextRef()
{
	return &mptr_DeviceContext;
}
ID3D11DeviceContext * CDeviceContext::GetDeviceContext()
{
	return 	mptr_DeviceContext;
	;
}
#elif USING_OPEN_GL
#endif


