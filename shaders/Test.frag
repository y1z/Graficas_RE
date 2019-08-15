#version 430 core

// Interpolated values from the vertex shaders
in vec2 TexCoord;
in vec3 PosWorld ;
in vec3 Norm;
in vec4 LightPos;

// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;

//retuns the normalized value between 2 vector of 3 components
vec3 GetHalfVector(vec3 Vec1, vec3 Vec2)
{
vec3 Result = normalize(Vec1 + Vec2);
return Result;
}
// Values that stay constant for the whole mesh.
//uniform sampler2D myTextureSampler;

void main()
{
	// Output color = color of the texture at the specified UV
	//color = texture( myTextureSampler, UV ).rgb;
  vec4 diffuse = texture(myTextureSampler,TexCoord);

  vec3 LightDir = normalize(LightPos.xyz - PosWorld);

  color = diffuse + max(0.0f,dot(LightDir, Norm) ); //
}
