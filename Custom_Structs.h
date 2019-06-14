#pragma once

#include "glm.hpp"

struct VertexWithTexture
{
	glm::vec3 Pos;
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
