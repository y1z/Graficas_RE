#pragma once
#include "glm.hpp"

using uint = unsigned int;

struct VertexWithTexture
{
	glm::vec4 Pos;
	glm::vec3 Norm;
	glm::vec2 TexCoord;
};

struct GlNeverChangeBuf
{
	glm::mat4x4 ViewMatrix;
};

struct GlChangeOnResizeBuf
{
	glm::mat4x4 ProjectionMatrix;
};

struct GlChangesEveryFrameBuf
{
	glm::mat4x4 WorldMatrix;
	float Color[4];
};

