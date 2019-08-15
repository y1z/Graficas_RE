Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbNeverChanges : register(b0)
{
    matrix View;
    // where the camera is looking at 
    // note the CPU already normalizes this vector 
    float3 FrontVector;
};

cbuffer cbChangeOnResize : register(b1)
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b2)
{
    matrix World;
    float4 vMeshColor;
};

cbuffer LightBuffer : register(b3)
{
    float4 LightPos;
    float4 LightColor;
    float3 LightDir;
    float LightIntensity;
};

// Vertex Output
struct VS_OUTPUT
{
    float4 Pos : POSITION0;
    float3 Norm : NORMAL0;
    float2 Tex : TEXCOORD;
};
// Pixel input 
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
    // light steel blue
    float3 ambientColor = { 176 / 255, 196 / 255, 222 / 255 };

    float3 EmissiColor = txDiffuse.Sample(samLinear, input.Tex).rgb;

    float3 LightDirNorm = normalize(-LightDir);

    float3 HalfVector = normalize(LightDirNorm + FrontVector);

    float3 NormNormal = normalize(input.Norm);

    float3 ReflectVector = reflect(LightDir,input.Norm);
    // normal moved into world space 
    float3 NormWorld = normalize(mul(float4(input.Norm, 1.0f), World));

    float3 Specular = pow(saturate(dot(NormNormal, HalfVector)), 1.0f) * max(0.01f, vMeshColor.z);

   // float3 Result = pow(dot(HalfVector, NormWorld), 20) /*abs(vMeshColor.z) * 3)*/; // max(0.01f, vMeshColor.z) * 30 /*max(0.0f, vMeshColor.z)*/);
    // get the textures 
    float4 TexSample = txDiffuse.Sample(samLinear, input.Tex);

    float3 Diffuse = saturate(dot(NormNormal, LightDirNorm)) * max(0.01f, vMeshColor.z);
   // diffuse = diffuse + saturate(dot(LightPos, input.Norm));
    float Alpha = TexSample.a;
    float3 Color = (saturate(ambientColor + Diffuse) * TexSample.rgb + Specular) * LightColor + EmissiColor;
    return float4(Color, Alpha); // + float4(ambientColor, 1.0f) + Specular; // + saturate(dot(LightPos, input.Norm));
}