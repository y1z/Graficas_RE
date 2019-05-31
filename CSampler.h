#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include <cfloat>// for  the macro FLT_MAX

//! this is my own descriptor struct that work as an intermediate between. mi and directX offset
struct SSampler
{
	int Filter;
	int ExampleAddressU;
	int ExampleAddressV;
	int ExampleAddressW;
	float MipsOffset;
	char AnisotropicLevel = 1;
	int ComparisonType;
	float PossibleBorderColor[4];
	float LowerRangedDetail; //lowest range for mip-maps 
	float UpperRangedDetail;// heights level of detail for mip-maps
};

class CSampler
{
public:// constructors 
	CSampler();
	~CSampler();
public:// functions 

#ifdef USING_DIRECTX
	void ConvertDxToSampler(D3D11_SAMPLER_DESC &DxDescriptor);
	/*! Requires that you used one of two functions "ConvertDxToSampler" or "SetUpSampler"*/
	D3D11_SAMPLER_DESC ConvertSamplerToDx();

	ID3D11SamplerState* GetSampler();
	ID3D11SamplerState** GetSamplerRef();

#endif // USING_DIRECTX

	/*! this is a function that set up every single variable int the type "SSampler"
\param Filter [in] the type of filter used
\param AddressU [in] the x axis in a uv map
\param AddressV [in] the y axis in a uv map
\param AddressW [in] the z axis in a uv map
\param ComperasionType [in] the comparison desired
\param MipsOffSet [in] an offset for the mips system
\param AnisotropicLevel [in] how much anisotropic filtering do you want(assuming you have anisotropic filtering )
\param Color4. [in] A color for a possible border.
\param LowerRange [in] what's the minimal range for level of detail
\param UpperRange [in] what's Maximum range for level of detail*/
	void SetUpSampler(int Filter, int AddressU,
		int AddressV, int AddressW,
		int ComperasionType, float MipsOffset = 0.0f,
		char AnisotropicLevel = 1, float *Color4 = nullptr,
		float LowerRange = 0.0f, float UpperRange = FLT_MAX);

private:// variable

#ifdef USING_DIRECTX
	ID3D11SamplerState* mptr_Sampler = nullptr;
#endif // USING_DIRECTX

	SSampler m_Descriptor;
	bool isSamplerReady = false;
};

