#version 430 core
// Input vertex data, different for all executions of this shader.
//layout(location = 0) in vec4 vertexPosition_modelspace;
//layout(location = 1) in vec2 vertexUV;

  struct VS_Input{
  vec4 Pos;// layout = 0
  vec3 Norm;// layout = 1
  vec2 UV;// layout = 2
};

struct WorldAndLight {
  mat4 World;
  vec4 LightPos;
};

layout(location = 0) in VS_Input Result;

// Output data ; will be interpolated for each fragment.
out vec2 TexCoord;
out vec3 PosWorld ;
out vec3 Norm;
out vec4 LightPos;
// it's the Projection , view , trasform rolled into one
uniform mat4 MVP;
// this is for when we need the projection matrix by it self
uniform mat4 u_Projection;
// this if for when we need the view matrix by it self
uniform mat4 u_View;

uniform WorldAndLight u_World;

float PI = 3.1415f;

void main(){
 //u_World.World * u_View * u_Projection  *
vec4 position = Result.Pos * u_World.World;
PosWorld = position.xyz;
position = position * u_View;
position = position * u_Projection;
// using this becuase the model won't show up without it
gl_Position = MVP * Result.Pos;

 LightPos = u_World.LightPos;
 //u_Projection * vec4(1,1,1,1);
 //u_View * vec4(1,1,1,1);

	// UV of the vertex. No special space for this one.
	TexCoord = Result.UV;

  //
  Norm = vec3(normalize(vec4(Result.Norm,1.0f) * u_World.World)) * max(3.0f,0.1f);

}
