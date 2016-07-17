#include "stdafx.h"
#include "u_process.h"

#include "u_system.h"

inline bool process::bitness::is_x64()
{
#if defined(_WIN64)			// x64 ����. �������
	return true;
#elif defined(_WIN32)		// x32 ����. �������
	return false;
#else
	#error UNSUPPORTED_PLATFORM
#endif
}