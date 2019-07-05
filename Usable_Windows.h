#pragma once

#if USING_OPEN_GL
#undef USING_DIRECTX
#endif

/*! give me the ability to use 
variable or function with the word 'min' or 'max'*/
#define NOMINMAX

/*! Don't include stuff I don't care about  */
#define WIN32_LEAN_AND_MEAN


#include <windows.h> 