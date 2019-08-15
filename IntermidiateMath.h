#pragma once
#include <glm/glm.hpp>

/*! 
\brief this class is going to be used to 
help translate between the math api's GLM and xnamath Math*/
namespace Math {

	class IntermidiateMath
	{
	public:
		IntermidiateMath();
		~IntermidiateMath();


		void TraslationMat4x4(void * First,void * Arg);


	};

}