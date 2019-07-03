#include"CInputLayout.h"
#include "Utility/ErrorHandlingGrafics.h"
#include "CVertexShader.h"
#include <fstream>

CInputLayout::CInputLayout()
{}


CInputLayout::~CInputLayout()
{
	if (mptr_InputLayoutDX) { mptr_InputLayoutDX->Release(); }
}

std::vector<SInputDesc> CInputLayout::GetDescriptionVec()
{
	return m_InputLayouts;
}

#ifdef USING_DIRECTX

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


bool CInputLayout::ReadShaderDataDX(CVertexShader& ShaderData, int ShaderInputData)
{
	// checking for errors 
	HRESULT hr = S_FALSE;

	ID3D11ShaderReflection *ReflectorShader = nullptr;

	// to init a shader Reflection
	hr = D3DReflect(ShaderData.GetVertexShaderData()->GetBufferPointer(), ShaderData.GetVertexShaderData()->GetBufferSize(),
		IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&ReflectorShader));

	if (CheckForError(hr))
	{
		return false;
	}

	///---------------------------------
	D3D11_SHADER_DESC ShaderDesc;
	// to know how long to make the for loop
	ReflectorShader->GetDesc(&ShaderDesc);

	for (uint32_t i = 0; i < ShaderDesc.InputParameters; ++i)
	{
		// where to store the data i need for the input layout 
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		ReflectorShader->GetInputParameterDesc(i, &paramDesc);
		SInputDesc InputLayout;

		InputLayout.Name = paramDesc.SemanticName;
		InputLayout.Index = paramDesc.SemanticIndex;
		InputLayout.DataArrangement = 0;
		InputLayout.Slots = 0;

		if (i != 0)
		{
			InputLayout.Alignment = ShaderDesc.FloatInstructionCount;
		}
		else
		{
			InputLayout.Alignment = 0;
		}
		InputLayout.InputDataType = ShaderInputData;

		// determine DXGI format 
		//https://docs.microsoft.com/en-us/windows/desktop/api/d3dcommon/ne-d3dcommon-d3d_register_component_type
		// 
		if (paramDesc.Mask == 1)
		{
			// 1 component values
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) { InputLayout.Format = DXGI_FORMAT_R32_UINT; }
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) { InputLayout.Format = DXGI_FORMAT_R32_SINT; }
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) { InputLayout.Format = DXGI_FORMAT_R32_FLOAT; }
		}
		// 2 component values 
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) { InputLayout.Format = DXGI_FORMAT_R32G32_UINT; }
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) { InputLayout.Format = DXGI_FORMAT_R32G32_SINT; }
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) { InputLayout.Format = DXGI_FORMAT_R32G32_FLOAT; }
		}
		// 3 component values 
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) { InputLayout.Format = DXGI_FORMAT_R32G32B32_UINT; }
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) { InputLayout.Format = DXGI_FORMAT_R32G32B32_SINT; }
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) { InputLayout.Format = DXGI_FORMAT_R32G32B32_FLOAT; }
		}
		// 4 component values 
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				InputLayout.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				InputLayout.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{// this is just a temporary fix
				InputLayout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
		}
		else// when non of the others are recognized
		{
			// should work on everything
			InputLayout.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		m_InputLayouts.emplace_back(InputLayout);
	}
	// no longer needed 
	ReflectorShader->Release();
	return true;
}

/*! this is for use with directX */
std::vector<D3D11_INPUT_ELEMENT_DESC> CInputLayout::ConvertInputLayoutToDx()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> Result;

	/*MUST BE A REFERECE because if not the string data does not stay on 'ConvertedTo' type*/
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

ID3D11InputLayout * CInputLayout::GetInputLayout()
{
	return mptr_InputLayoutDX;
}

ID3D11InputLayout ** CInputLayout::GetInputLayoutRef()
{
	return &mptr_InputLayoutDX;
}

#endif // USING_DIRECTX