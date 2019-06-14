#pragma once

#include "..//Usable_Windows.h"

#if defined USING_DIRECTX
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
	if (hr == E_INVALIDARG) {
		return true; 
	}

	return false;
}

#elif //TODO_GL 

#endif // USING_DIRECTX