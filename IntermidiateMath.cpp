#include "IntermidiateMath.h"
#include "DirectXHeader.h"

namespace Math {

	IntermidiateMath::IntermidiateMath()
	{}


	IntermidiateMath::~IntermidiateMath()
	{}

	void IntermidiateMath::TraslationMat4x4(void * First, void * Arg)
	{
	// math
	#ifdef USING_DIRECTX
		XMMATRIX* Mat4 = static_cast<XMMATRIX*>(First);
		XMMATRIX* ARgMat4 = static_cast<XMMATRIX*>(Arg);
		CXMMATRIX UsedArg = *ARgMat4;
		*Mat4 = XMMatrixTranspose(UsedArg);

	#elif USING_OPEN_GL

	#endif // USING_DIRECTX
	}

}