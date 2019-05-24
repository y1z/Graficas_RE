#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include <vector>
#include <string_view>
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
	std::string_view Name;
	uint32_t Index;
	char Slots;
	int Format;
	int32_t Alignment;
	int DataArrangement;
	int InputDataType;
};

class CInputLayout
{
public:
	CInputLayout();
	~CInputLayout();
public:
	std::vector<SInputDesc> GetDescirtionVec();
#ifdef USING_DIRECTX
	void ConvertDxToInputLayout(D3D11_INPUT_ELEMENT_DESC *Layout, int amout);
	std::vector<D3D11_INPUT_ELEMENT_DESC> ConvertInputLayoutToDx();
#endif // USING_DIRECTX
	std::vector<SInputDesc> m_InputLayouts;
private:

	SInputDesc m_IntermediateDesc;

#ifdef USING_DIRECTX
	ID3D11InputLayout* mdx_InputLayout = nullptr;
#elif
#endif // USING_DIRECTX

};

