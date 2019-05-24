#include"CInputLayout.h"

CInputLayout::CInputLayout()
{}


CInputLayout::~CInputLayout()
{
	if (mdx_InputLayout) { mdx_InputLayout->Release(); }
}

std::vector<SInputDesc> CInputLayout::GetDescirtionVec()
{
	return m_InputLayouts;
}

void CInputLayout::ConvertDxToInputLayout(D3D11_INPUT_ELEMENT_DESC *Layout, int amount)
{

	for (int i = 0; i < amount; ++i)
	{
		SInputDesc Converted;
		Converted.Name = Layout->SemanticName;
		Converted.Index = Layout->SemanticIndex;
		Converted.Alignment = Layout->AlignedByteOffset;
		Converted.Format = Layout->Format;
		Converted.Slots = Layout->InputSlot;
		Converted.InputDataType = Layout->InputSlotClass;
		Converted.DataArrangement = Layout->InstanceDataStepRate;

		Layout++;

		m_InputLayouts.emplace_back(Converted);
	}


}
/*! this is for use with directX */
std::vector<D3D11_INPUT_ELEMENT_DESC> CInputLayout::ConvertInputLayoutToDx()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> Result;

	/*MUST BE A REFERECE because if not the string data does not stay on 'ConvertedTo'*/
	for (SInputDesc &Intermediate : m_InputLayouts)
	{
		D3D11_INPUT_ELEMENT_DESC ConvertedTo;
		ConvertedTo.SemanticName = Intermediate.Name.c_str();
		ConvertedTo.SemanticIndex = Intermediate.Index;
		ConvertedTo.Format = static_cast<DXGI_FORMAT>(Intermediate.Format);
		ConvertedTo.InputSlot = Intermediate.Slots;
		ConvertedTo.AlignedByteOffset = Intermediate.Alignment;
		ConvertedTo.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(Intermediate.InputDataType);
		ConvertedTo.InstanceDataStepRate = Intermediate.DataArrangement;

		Result.emplace_back(ConvertedTo);
	}


	return Result;
}
