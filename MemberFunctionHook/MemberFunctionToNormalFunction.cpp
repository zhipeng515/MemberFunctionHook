#include "stdafx.h"
#include "MemberFunctionToNormalFunction.h"

std::vector<MemberFunctionToNormalFunction*> MemberFunctionToNormalFunction::FunctionVector;

class DestructStaticVector
{
public:
	~DestructStaticVector()
	{
		MEMBERFUNCTION_TO_NORMALFUNCTION_RELEASEALL();
	}
};
DestructStaticVector destructStaticVector;