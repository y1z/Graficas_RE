#pragma once

#include "..//OpenglHeader.h"
#include "..//Usable_Windows.h"
#include <string>
#include <sstream>
using namespace  std::string_literals;

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
	bool IsErrorResult = false;
	while (GLenum Error = glGetError())
	{
		IsErrorResult = true;
		std::stringstream ConvertToHex;

		ConvertToHex << std::hex << (unsigned int)Error;

		std::string Message = "\n ---- ERROR HAS OCURRED :< ";

		Message += ConvertToHex.str();
		Message += "> \n";
		
		OutputDebugStringA( Message.c_str());
	}
	return IsErrorResult;
}


#endif // USING_DIRECTX