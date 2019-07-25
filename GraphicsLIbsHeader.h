#pragma once
//! Controls if your using Open_GL or DirectX 
//#define USING_OPEN_GL 1

// use open_Gl
#ifdef USING_OPEN_GL 1

#undef USING_DIRECTX

#endif // USING_OPEN_GL
///////////////////////////////////////////////
// use DirectX 
#ifndef USING_OPEN_GL

#define USING_DIRECTX 1
#undef USING_OPEN_GL

#endif // !USING_DIRECTX

// when open_gl is NOT in use, chose directX instead. 
#ifdef USING_OPEN_GL
#include  "OpenglHeader.h"
#elif USING_DIRECTX
#include "DirectXHeader.h"
#endif // USING_OPEN_GL
