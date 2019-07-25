#pragma once

#include "..//OpenglHeader.h"
#include "..//Usable_Windows.h"
#include <string>

#ifdef USING_DIRECTX
/*! tell's if there is an error */
static bool CheckForError(HRESULT hr)
{
	if (FAILED(hr))
	{
		return true;
	}
	if (hr == S_FALSE)
	{
		return true;
	}
	if (hr == E_INVALIDARG)
	{
		return true;
	}

	return false;
}

#elif USING_OPEN_GL//TODO_GL 

//! remove all the error that are currently present 
static void GlRemoveAllErrors()
{

	while (glGetError() != GL_NO_ERROR)
	{}
}

static bool GlCheckForError()
{

	while (GLenum Error = glGetError())
	{

		std::string Result = std::to_string(Error).c_str();

		OutputDebugStringA(Result.c_str());
	}
	return false;
}


#endif // USING_DIRECTX