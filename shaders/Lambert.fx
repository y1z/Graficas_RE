//float4x4 matWorldViewProjection;
//float4x4 matWorld;
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);
// view
cbuffer cbNeverChanges : register(b0)
{
    matrix View;
   // where the camera is looking at 
    float3 AtVector;
};
// Projection 
cbuffer cbChangeOnResize : register(b1)
{
    matrix Projection;
};
// world 
cbuffer cbChangesEveryFrame : register(b2)
{
    matrix World;
    float4 vMeshColor;
};

// Vertex Output
struct VS_OUTPUT
{
    float4 Pos : POSITION0;
    float3 Norm : NORMAL0;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION0;
    float3 Norm : NORMAL0;
    float2 Tex : TEXCOORD;
};

PS_INPUT vs_main(VS_OUTPUT input)
{
    PS_INPUT Out = (PS_INPUT) 0;
    // move to the world dimension.
    Out.Pos = mul(input.Pos, World); // transform Position
    // move to the world View 
    Out.Pos = mul(Out.Pos, View);
    // projection
    Out.Pos = mul(Out.Pos, Projection);
    // transform 
    //converting a value that float3 to float4 
    Out.Norm = normalize(mul(float4(input.Norm, 1.0f), World));
    // make sure i can see the textures 
    Out.Tex = input.Tex;
    return Out;
}

// Pixel Shader 
float4 ps_main(PS_INPUT input) : SV_Target
{
    float3 m_light = { vMeshColor.xyz };
    //float4 color = txDiffuse.Sample(samLinear, input.Tex);
    float4 diffuse = txDiffuse.Sample(samLinear, input.Tex);
    return diffuse + saturate(dot(m_light, input.Norm));
}