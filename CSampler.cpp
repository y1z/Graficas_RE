#include "CSampler.h"


CSampler::CSampler()
{}


CSampler::~CSampler()
{
#if USING_DIRECTX
	if (mptr_Sampler != nullptr) { mptr_Sampler->Release(); }
#endif // USING_DIRECTX

}
#if USING_DIRECTX

void CSampler::ConvertDxToSampler(D3D11_SAMPLER_DESC & DxDescriptor)
{
	m_Descriptor.Filter = DxDescriptor.Filter;
	m_Descriptor.ExampleAddressU = DxDescriptor.AddressU;
	m_Descriptor.ExampleAddressV = DxDescriptor.AddressV;
	m_Descriptor.ExampleAddressW = DxDescriptor.AddressW;
	m_Descriptor.MipsOffset = DxDescriptor.MipLODBias;
	m_Descriptor.AnisotropicLevel = DxDescriptor.MaxAnisotropy;
	m_Descriptor.ComparisonType = DxDescriptor.ComparisonFunc;
	for (int i = 0; i < 4; ++i)
	{
		m_Descriptor.PossibleBorderColor[i] = DxDescriptor.BorderColor[i];
	}
	m_Descriptor.LowerRangedDetail = DxDescriptor.MinLOD;
	m_Descriptor.UpperRangedDetail = DxDescriptor.MaxLOD;

	isSamplerReady = true;
}

D3D11_SAMPLER_DESC CSampler::ConvertSamplerToDx()
{
	D3D11_SAMPLER_DESC Result;
	Result.Filter = static_cast<D3D11_FILTER>(m_Descriptor.Filter);

	Result.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE> (m_Descriptor.ExampleAddressU);
	Result.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE> (m_Descriptor.ExampleAddressV);
	Result.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE> (m_Descriptor.ExampleAddressW);

	Result.MipLODBias = m_Descriptor.MipsOffset;
	Result.MaxAnisotropy = m_Descriptor.AnisotropicLevel;
	Result.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(m_Descriptor.ComparisonType);

	for (int i = 0; i < 4; ++i)
	{
		Result.BorderColor[i] = m_Descriptor.PossibleBorderColor[i];
	}

	Result.MinLOD = m_Descriptor.LowerRangedDetail;
	Result.MaxLOD = m_Descriptor.UpperRangedDetail;

	return Result;
}

ID3D11SamplerState * CSampler::GetSampler()
{
	return mptr_Sampler;
}

ID3D11SamplerState ** CSampler::GetSamplerRef()
{
	return &mptr_Sampler;
}

#endif // USING_DIRECTX

void CSampler::SetUpSampler(int Filter, int AddressU,
	int AddressV, int AddressW,
	int ComperasionType, float MipsOffset,
	char AnisotrapicLevel, float * Color4,
	float LowerRange, float UpperRange)
{
	m_Descriptor.Filter = Filter;
	m_Descriptor.ExampleAddressU = AddressU;
	m_Descriptor.ExampleAddressV = AddressV;
	m_Descriptor.ExampleAddressW = AddressW;

	m_Descriptor.ComparisonType = ComperasionType;
	m_Descriptor.MipsOffset = MipsOffset;
	m_Descriptor.AnisotropicLevel = AnisotrapicLevel;

	if (Color4 == nullptr)
	{
		float Purple[4] = { 0.5f,0,0.5f,1.0f };

		for (int i = 0; i < 4; ++i)
		{
			m_Descriptor.PossibleBorderColor[i] = Purple[i];
		}

	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			m_Descriptor.PossibleBorderColor[i] = Color4[i];
		}

	}
	m_Descriptor.LowerRangedDetail = LowerRange;
	m_Descriptor.UpperRangedDetail = UpperRange;

	isSamplerReady = true;
}
