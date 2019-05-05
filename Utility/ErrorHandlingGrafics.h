#pragma once
#include "..//Usable_Windows.h"

#if defined USING_DIRECTX
/*! tell's if there is an error */
bool CheckForError(HRESULT hr)
{
	if (FAILED(hr)) 
	{
		return true;
	}

	return false;
}



#elif //TODO_GL 



#endif 