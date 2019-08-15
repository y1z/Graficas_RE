#pragma once
#include "Usable_Windows.h"
#include  "GraphicsLIbsHeader.h"
#include <vector>

#include <string_view>
class CVertexShader;

/*struct D3D11_INPUT_ELEMENT_DESC
		{
		LPCSTR SemanticName;
		UINT SemanticIndex;
		DXGI_FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
		} 	D3D11_INPUT_ELEMENT_DESC;*/

		/*! this is an abstraction of the D3D11_INPUT_ELEMENT_DESC type
		 this will be used to communicate with directX and OpenGl.*/
struct SInputDesc
{
	std::string Name;
	uint32_t Index;
	int Slots;
	int Format;
	uint32_t Alignment;
	int DataArrangement;
	int InputDataType;
};
/*\file
\class CInputLayout "CInputLayout.h"
\brief this class acts as a in-between the native DirectX input-layout and mi own input layout
*/

class CInputLayout
{
public:
	CInputLayout();
	~CInputLayout();
public:
	//! returns my input layout 
	std::vector<SInputDesc> GetDescriptionVec();
#ifdef USING_DIRECTX
	//! take the directX input layout and convert that to MY input-layout struct
	void ConvertDxToInputLayout(D3D11_INPUT_ELEMENT_DESC *Layout, int amout);
	/*! used to create a dynamic input-layout 
	\param [in] ShaderData is used to get the necessary from the shader 
	\param [in] ShaderInputData used to know what type of data I'm receiving from the buffer(D3D11_INPUT_CLASSIFICATION) */
	bool ReadShaderDataDX(CVertexShader& ShaderData,int ShaderInputData);
	//! convert MY input layout to directX native input layout plus returns a vector
	std::vector<D3D11_INPUT_ELEMENT_DESC> ConvertInputLayoutToDx();
	// 
	ID3D11InputLayout* GetInputLayout();
	ID3D11InputLayout**GetInputLayoutRef();
#endif // USING_DIRECTX
private:
	std::vector<SInputDesc> m_InputLayouts;

#ifdef USING_DIRECTX
	ID3D11InputLayout* mptr_InputLayoutDX = nullptr;
#else
#endif // USING_DIRECTX

};

