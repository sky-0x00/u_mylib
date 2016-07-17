#ifndef u_math
#define u_math

#include "u_types.h"

namespace math { 
	template <typename type> type abs(type x)
	{
		return (x < 0) ? -x : x;
	}
	template <typename type> int08 sign(type x)
	{
		if (x > 0) return  1;
		if (x < 0) return -1;
		return 0;
	}
}

#endif u_math