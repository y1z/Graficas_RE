#pragma once

#ifndef USING_OPEN_GL
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif // USING_OPEN_GL
#include "Resource.h"

#ifdef USING_OPEN_GL
#undef USING_DIRECTX
#endif

