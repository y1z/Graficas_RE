#pragma once
#include "DirectXHeader.h"
#include <cinttypes>

class CDeviaceContext
{
public:
	CDeviaceContext();
	~CDeviaceContext();
public:// functions 

	/*! setting render target
\param totalView [in] to know how many view to set
\param RenderTragetView [out] The Resulting RenderTraget (ID3D11RenderTargetView*)
\param DepthStencilView [out] paints things on screen (ID3D11DepthStencilView*)*/
	void OMSetRenderTargets(int32_t totalViews, void *RenderTrageView, void* DepthStencilView);

	/*! set the viewports
\param TotalViews [in] how many view there are
\param ViewPort [out] the View port (D3D11_VIEWPORT*)*/
	void RSSetViewports(int32_t TotalViews, void* ViewPort);

	/*! For the InputLyout
\param InputLayout [in] tells the machine how to interpret data ( ID3D11InputLayout*)
*/
	void IASetInputLayout(void * InputLayout);

	/*! Sets the VertexBuffer
\param StartSlot [in] placement
\param TotalBuffer [in] to know how many buffer to set
\param VertexBuffer [out] The vertex-buffer thats affected(ID3D11Buffer *)
\param ptr_Stride [in] how big the vertex data type is
\param ptr_offset [in] the distance between data
*/
	void IASetVertexBuffers(int32_t StratSlot, int32_t TotalBuffer, void* VertexBuffer, void *ptr_Stride, void*ptr_offset);

	/*! Set the IndexBuffer
\param IndexBuffer [out] the vertex buffer that's set
\param Format [in] how to interpret the IndexBuffer
\param Offset [in] distance between data we dont care for */
	void IASetIndexBuffer(void *VertexBuffer, int Format, int Offset);

	/*! set the topology
	\param Topology [in] dictates how the geometry is going to be drawn */
	void IASetPrimitiveTopology(int Topology = 4);

	/*! update some thing
\param DistResource [out] will receive an update(ID3D11Resource *)
\param OriginResrouce [in] where it came from
\param ReservedCopyLocation [out] A specified place to copy information
\param*/
	void UpdateSubresource(void *DistResource, void* OriginResrouce, int32_t IndexSubResourceFinder, void *ReservedCopyLocation = nullptr, uint32_t PitchRow = 0, uint32_t PitchDepth = 0);

	/*! dictates what color the RenderTraget
\param RenderTraget[out] the render target (ID3D11RenderTargetView*)
\param Color [in] dictates which color the screen will clear to*/
	void ClearRenderTargetView(void *RenderTraget, float *Color = nullptr);

	/*! Clear the Depth buffer and StencilView
\param DepthStnecilView [out] Cleared DepthStencilView (ID3D11DepthStencilView*)
\param Flags [in] aditional options
\param DepthBfferClear [in] floatthing point value
\param ClearStencil [in] Color /to crear.*/
	void ClearDepthStencilView(void * DepthStencilView, int Flags, float DepthBufferClear, int8_t ClearStencil);

	/*! Clear the Depth buffer and StencilView
\param DepthStnecilView [out] Cleared DepthStencilView (ID3D11DepthStencilView*)
\param Flags [in] aditional options
\param DepthBfferClear [in] floatthing point value
\param ClearStencil [in] Color /to crear.*/
	void VSSetShader(void*VertexShader);

	/*! Sets the constant Buffer
\param StartSlot[in] where will the buffer be placed
\param TotalBuffers [in] for knowing how many buffer there are
.*/
	void VSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, void *Buffer);

	/*! Set the Pixel Shader
	\param PixelShader[out] The Resulting Shader*/
	void PSSetShader(void *PixelShader);

	void PSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, void *Buffer);
	/*!
\param StratSlot [in] dictates where you are
\param TotalViews [in] How many view are there
\param ShaderResourceView
*/
	void PSSetShaderResources(int32_t StratSlot, int32_t TotalViews, void *ShaderResourceView);
	/*!
\param StratSlot [in] dictates where you are in compaction to others
\param TotalSamplers [in] dictates how many Samplers there are .
*/
	void PSSetSamplers(int32_t StartSlot, int32_t TotalSamplers, void* Sampler);
	/*!
\param TotalIndexs[in] how many triangles there are
\param StartIndex[in]
\param IndexDiff[in]*/
	void DrawIndexed(int32_t TotalIndexs, int32_t StartIndex, int32_t IndexDiff = 0);

#if defined(USING_DIRECTX)
	ID3D11DeviceContext ** GetDeviceContextRef();
	ID3D11DeviceContext * GetDeviceContext();
#elif
#endif
public:
#ifdef USING_DIRECTX
	ID3D11DeviceContext* mptr_DeviceContext = nullptr;

#endif // USING_DIRECTX

};

