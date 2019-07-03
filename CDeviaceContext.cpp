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

CDeviaceContext::CDeviaceContext()
{}


CDeviaceContext::~CDeviaceContext()
{
	if (mptr_DeviceContext != nullptr) { mptr_DeviceContext->Release(); }
}


void CDeviaceContext::OMSetRenderTargets(int32_t totalViews, CRenderTragetView &renderTragetView, CDepthStencilView &DepthStencilView)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->OMSetRenderTargets(totalViews,
		static_cast<ID3D11RenderTargetView**>(renderTragetView.GetRenderTragetRef()),
		static_cast<ID3D11DepthStencilView*>(DepthStencilView.GetDepthStencilView()));
#elif
#endif
}

void CDeviaceContext::RSSetViewports(int32_t TotalViews, CViewPort &ViewPort)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->RSSetViewports(TotalViews, static_cast<D3D11_VIEWPORT*>(ViewPort.GetViewPortRef()));
#elif
#endif

}


void CDeviaceContext::IASetInputLayout(CInputLayout &InputLayout)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->IASetInputLayout(static_cast<ID3D11InputLayout*>(InputLayout.GetInputLayout()));
#elif
#endif

}

void CDeviaceContext::IASetVertexBuffers(int32_t StratSlot, int32_t TotalBuffer, CBuffer &VertexBuffer, uint32_t Stride, uint32_t Offset)
{
#if defined(USING_DIRECTX)
	UINT ConversionForStride = Stride;
	UINT ConversionForOffset = Offset;

	mptr_DeviceContext->IASetVertexBuffers(StratSlot, TotalBuffer,
		static_cast<ID3D11Buffer **>(VertexBuffer.GetBufferRef()), static_cast<UINT *>(&ConversionForStride),
		static_cast<UINT *>(&ConversionForOffset));
#elif
#endif
}

void CDeviaceContext::IASetIndexBuffer(CBuffer& IndexBuffer, int Format, int Offset)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->IASetIndexBuffer(static_cast<ID3D11Buffer*>(IndexBuffer.GetBuffer()),
		static_cast<DXGI_FORMAT>(Format), Offset);
#elif
#endif
}

/*! set the topology
\param Topology [in] dictates how the geometry is going to be drawn */
void CDeviaceContext::IASetPrimitiveTopology(int Topology)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(Topology));
#elif
#endif
}

/*TODO : make many overloads for this function */
void CDeviaceContext::UpdateSubresource(CBuffer &DistResource, void* OriginResrouce,
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
#elif
#endif
}

/*! dictates what color the RenderTraget
\param RenderTraget[out] the render target (ID3D11RenderTargetView*)
\param Color [in] dictates which color the screen will clear to*/
void CDeviaceContext::ClearRenderTargetView(CRenderTragetView &RenderTraget, float * Color)
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
#elif
#endif

}

void CDeviaceContext::ClearDepthStencilView(CDepthStencilView &DepthStencilView, int Flags, float DepthBufferClear, int8_t ClearStencil)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->ClearDepthStencilView(static_cast<ID3D11DepthStencilView*>(DepthStencilView.GetDepthStencilView())
		, Flags
		, DepthBufferClear
		, ClearStencil);
#elif
#endif
}

void CDeviaceContext::VSSetShader(CVertexShader &VertexShader)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->VSSetShader(static_cast<ID3D11VertexShader*>(VertexShader.GetVertexShader()), nullptr, 0);
#elif
#endif
}

void CDeviaceContext::VSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, CBuffer *Buffer)
{
#if defined(USING_DIRECTX)
	for (int i = 0; i < TotalBuffers; ++i)
	{
		mptr_DeviceContext->VSSetConstantBuffers(StartSlot, TotalBuffers, static_cast<ID3D11Buffer**>(Buffer[i].GetBufferRef()));

	}
#elif
#endif
}

void CDeviaceContext::PSSetShader(CPixelShader &PixelShader)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetShader(static_cast<ID3D11PixelShader*>(PixelShader.GetPixelShader()), nullptr, 0);
#elif
#endif
}

void CDeviaceContext::PSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, CBuffer *Buffer)
{
#if defined(USING_DIRECTX)
	for (int i = 0; i < TotalBuffers; ++i)
	{
		mptr_DeviceContext->PSSetConstantBuffers(StartSlot, TotalBuffers, static_cast<ID3D11Buffer**>(Buffer[i].GetBufferRef()));
	}
#elif
#endif
}

void CDeviaceContext::PSSetShaderResources(int32_t StratSlot, int32_t TotalViews, CShaderResourceView &ShaderResourceView)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetShaderResources(StratSlot, TotalViews, static_cast<ID3D11ShaderResourceView**>(ShaderResourceView.GetResourceViewRef()));
#elif
#endif
}

void CDeviaceContext::PSSetSamplers(int32_t StartSlot, int32_t TotalSamplers, CSampler &Sampler)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetSamplers(StartSlot, TotalSamplers, static_cast<ID3D11SamplerState**>(Sampler.GetSamplerRef()));
#elif
#endif
}

void CDeviaceContext::DrawIndexed(int32_t TotalIndexs, int32_t StartIndex, int32_t IndexDiff)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->DrawIndexed(TotalIndexs, StartIndex, IndexDiff);
#elif
#endif
}

#if defined(USING_DIRECTX)
ID3D11DeviceContext **CDeviaceContext::GetDeviceContextRef()
{
	return &mptr_DeviceContext;
}
ID3D11DeviceContext * CDeviaceContext::GetDeviceContext()
{
	return 	mptr_DeviceContext;
	;
}
#elif
#endif


