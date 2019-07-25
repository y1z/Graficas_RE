#pragma once
#include "GraphicsLIbsHeader.h"

#include <cinttypes>
class CRenderTragetView;
class CDepthStencilView;
class CViewPort;
class CInputLayout;
class CBuffer;
class CVertexShader;
class CPixelShader;
class CShaderResourceView;
class CSampler;

class CDeviceContext
{
public:
	CDeviceContext();
	~CDeviceContext();
public:// functions 

	/*! setting render target
\param totalView [in] to know how many view to set
\param RenderTragetView [out] The Resulting RenderTraget (ID3D11RenderTargetView*)
\param DepthStencilView [out] paints things on screen (ID3D11DepthStencilView*)*/
	void OMSetRenderTargets(int32_t totalViews, CRenderTragetView &renderTragetView, CDepthStencilView &DepthStencilView);

	/*! set the viewports
\param TotalViews [in] how many view there are
\param ViewPort [out] the View port (D3D11_VIEWPORT*)*/
	void RSSetViewports(int32_t TotalViews, CViewPort &ViewPort);

	/*! For the InputLyout
\param InputLayout [in] tells the machine how to interpret data ( ID3D11InputLayout*)
*/
	void IASetInputLayout(CInputLayout &InputLayout);

	/*! Sets the VertexBuffer
\param StartSlot [in] placement
\param TotalBuffer [in] to know how many buffer to set
\param VertexBuffer [out] The vertex-buffer thats affected(ID3D11Buffer *)
\param ptr_Stride [in] how big the vertex data type is
\param ptr_offset [in] the distance between data
*/
	void IASetVertexBuffers(int32_t StratSlot, int32_t TotalBuffer, CBuffer &VertexBuffer, uint32_t Stride, uint32_t Offset);

	/*! Set the IndexBuffer
\param IndexBuffer [out] the vertex buffer that's set
\param Format [in] how to interpret the IndexBuffer
\param Offset [in] distance between data we dont care for */
	void IASetIndexBuffer(CBuffer &VertexBuffer, int Format, int Offset);

	/*! set the topology
	\param Topology [in] dictates how the geometry is going to be drawn */
	void IASetPrimitiveTopology(int Topology = 4);

	/*! update some thing
\param DistResource [out] will receive an update(ID3D11Resource *)
\param OriginResrouce [in] where it came from
\param ReservedCopyLocation [out] A specified place to copy information
\param*/
	void UpdateSubresource(CBuffer &DistResource, void* OriginResrouce, int32_t IndexSubResourceFinder, void *ReservedCopyLocation = nullptr, uint32_t PitchRow = 0, uint32_t PitchDepth = 0);

	/*! dictates what color the RenderTraget
\param RenderTraget[out] the render target (ID3D11RenderTargetView*)
\param Color [in] dictates which color the screen will clear to*/
	void ClearRenderTargetView(CRenderTragetView &RenderTraget, float *Color = nullptr);

	/*! Clear the Depth buffer and StencilView
\param DepthStnecilView [out] Cleared DepthStencilView (ID3D11DepthStencilView*)
\param Flags [in] aditional options
\param DepthBfferClear [in] floatthing point value
\param ClearStencil [in] Color /to crear.*/
	void ClearDepthStencilView(CDepthStencilView &DepthStencilView, int Flags, float DepthBufferClear, int8_t ClearStencil);

	/*! Clear the Depth buffer and StencilView
\param VertexShader [out] This is where we set the shader */
	void VSSetShader(CVertexShader &VertexShader);

	/*! Sets the constant Buffer
\param StartSlot[in] where will the buffer be placed
\param TotalBuffers [in] for knowing how many buffer there are
\param Buffer [in] the buffer that going te be set 
.*/
	void VSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, CBuffer *Buffer);

	/*! Set the Pixel Shader
	\param PixelShader[out] The Resulting Shader*/
	void PSSetShader(CPixelShader &PixelShader);

	void PSSetConstantBuffers(int32_t StartSlot, int32_t TotalBuffers, CBuffer *Buffer);
	/*!
\param StratSlot [in] dictates where you are
\param TotalViews [in] How many view are there
\param ShaderResourceView [in] allows you to see data from the shader.
*/
	void PSSetShaderResources(int32_t StratSlot, int32_t TotalViews, CShaderResourceView &ShaderResourceView);
	/*!
\param StratSlot [in] dictates where you are in compaction to others
\param TotalSamplers [in] dictates how many Samplers there are .
*/
	void PSSetSamplers(int32_t StartSlot, int32_t TotalSamplers, CSampler &Sampler);
	/*!
\param TotalIndexs [in] how many triangles there are
\param StartIndex [in] where to start drawing 
\param IndexDiff [in]  */
	void DrawIndexed(int32_t TotalIndexs, int32_t StartIndex, int32_t IndexDiff = 0);

	void DrawIndexed(CBuffer& IndexBuffer);

#ifdef USING_DIRECTX
	ID3D11DeviceContext ** GetDeviceContextRef();
	ID3D11DeviceContext * GetDeviceContext();
#else
	
#endif
private:
#ifdef USING_DIRECTX
	ID3D11DeviceContext* mptr_DeviceContext = nullptr;

#endif // USING_DIRECTX

};

