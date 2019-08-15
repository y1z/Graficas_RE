#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

using uint = unsigned int;
// position normal and UV / texcoord 
struct VertexPosNormTex
{
	glm::vec4 Pos;
	glm::vec3 Norm;
	glm::vec2 TexCoord;
};
// position and UV / texcoord 
struct VertexPosTex
{
	glm::vec4 Pos;
	glm::vec2 TexCoord;
};

struct Light
{
	glm::vec4 Pos;
	// default color is green
	glm::vec4 Color{0.0f,1.0f,0.0f,1.0f};
	glm::vec3 Direction;
	float Intensity{1.0f};
};

// view matrix 
struct GlNeverChangeBuf
{
	glm::mat4x4 ViewMatrix{ -1.0f };
	// where the camera is looking at 
	glm::vec3 FrontVector;
};
// Projection matrix 
struct GlChangeOnResizeBuf
{
	glm::mat4x4 ProjectionMatrix{-1.0f};
};
// world matrix 
struct GlChangesEveryFrameBuf
{
	glm::mat4x4 WorldMatrix{ -1.0f };
	glm::vec4 Color;
};

