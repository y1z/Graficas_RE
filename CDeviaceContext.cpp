#include "CDeviaceContext.h"



CDeviaceContext::CDeviaceContext()
{
}


CDeviaceContext::~CDeviaceContext()
{
	if (mptr_DeviceContext != nullptr) { mptr_DeviceContext->Release(); }
}


void CDeviaceContext::OMSetRenderTargets(int32_t totalViews, void * RenderTragetView, void * DepthStencilView)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->OMSetRenderTargets(totalViews,
		static_cast<ID3D11RenderTargetView**>(RenderTragetView),
		static_cast<ID3D11DepthStencilView*>(DepthStencilView));
#elif
#endif
}

void CDeviaceContext::RSSetViewports(int32_t TotalViews, void * ViewPort)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->RSSetViewports(TotalViews, static_cast<D3D11_VIEWPORT*>(ViewPort));
#elif
#endif

}


void CDeviaceContext::IASetInputLayout(void * InputLayout)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->IASetInputLayout(static_cast<ID3D11InputLayout*>(InputLayout));
#elif
#endif

}

void CDeviaceContext::IASetVertexBuffers(int32_t StratSlot, int32_t TotalBuffer, void * VertexBuffer, uint32_t Stride, uint32_t Offset)
{
#if defined(USING_DIRECTX)
	UINT ConversionForStride = Stride;
	UINT ConversionForOffset = Offset;

	mptr_DeviceContext->IASetVertexBuffers(StratSlot, TotalBuffer,
		static_cast<ID3D11Buffer **>(VertexBuffer), static_cast<UINT *>(&ConversionForStride),
		static_cast<UINT *>(&ConversionForOffset));
#elif
#endif
}

void CDeviaceContext::IASetIndexBuffer(void * IndexBuffer, int Format, int Offset)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->IASetIndexBuffer(static_cast<ID3D11Buffer*>(IndexBuffer),
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

void CDeviaceContext::UpdateSubresource(void *DistResource,
	void* OriginResrouce,
	int32_t IndexUpdateResource, void *ReservedCopyLocation,
	uint32_t PitchRow, uint32_t PitchDepth)
{
#if defined(USING_DIRECTX)
	if (ReservedCopyLocation == nullptr)
	{
		mptr_DeviceContext->UpdateSubresource(static_cast<ID3D11Resource *>(DistResource), IndexUpdateResource,
			nullptr, OriginResrouce,
			PitchRow, PitchDepth);
	}
	else {
		mptr_DeviceContext->UpdateSubresource(static_cast<ID3D11Resource *>(DistResource), IndexUpdateResource,
			static_cast<D3D11_BOX*>(ReservedCopyLocation), OriginResrouce,
			PitchRow, PitchDepth);
	}
#elif
#endif
}
/*! dictates what color the RenderTraget
\param RenderTraget[out] the render target (ID3D11RenderTargetView*)
\param Color [in] dictates which color the screen will clear to*/
void CDeviaceContext::ClearRenderTargetView(void * RenderTraget, float * Color)
{
#if defined(USING_DIRECTX)
	if (Color == nullptr)
	{
		/*Should be Purple*/
		float Purple[4] = { (255 / 136),(255 / 50),(255 / 249),1.0f };

		mptr_DeviceContext->ClearRenderTargetView(static_cast<ID3D11RenderTargetView*>(RenderTraget), Purple);
	}
	else {
		mptr_DeviceContext->ClearRenderTargetView(static_cast<ID3D11RenderTargetView*>(RenderTraget), Color);
	}
#elif
#endif

}

void CDeviaceContext::ClearDepthStencilView(void * DepthStencilView, int Flags, float DepthBufferClear, int8_t ClearStencil)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->ClearDepthStencilView(static_cast<ID3D11DepthStencilView*>(DepthStencilView)
		, Flags
		, DepthBufferClear
		, ClearStencil);
#elif
#endif
}

void CDeviaceContext::VSSetShader(void * VertexShader)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->VSSetShader(static_cast<ID3D11VertexShader*>(VertexShader), nullptr, 0);
#elif
#endif
}

void CDeviaceContext::VSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, void * Buffer)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->VSSetConstantBuffers(StartSlot, TotalBuffers, static_cast<ID3D11Buffer**>(Buffer));
#elif
#endif
}

void CDeviaceContext::PSSetShader(void * PixelShader)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetShader(static_cast<ID3D11PixelShader*>(PixelShader), nullptr, 0);
#elif
#endif
}

void CDeviaceContext::PSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, void * Buffer)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetConstantBuffers(StartSlot, TotalBuffers, static_cast<ID3D11Buffer**>(Buffer));
#elif
#endif
}

void CDeviaceContext::PSSetShaderResources(int32_t StratSlot, int32_t TotalViews, void * ShaderResourceView)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetShaderResources(StratSlot, TotalViews, static_cast<ID3D11ShaderResourceView**>(ShaderResourceView));
#elif
#endif
}

void CDeviaceContext::PSSetSamplers(int32_t StartSlot, int32_t TotalSamplers, void * Sampler)
{
#if defined(USING_DIRECTX)
	mptr_DeviceContext->PSSetSamplers(StartSlot, TotalSamplers, static_cast<ID3D11SamplerState**>(Sampler));
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


