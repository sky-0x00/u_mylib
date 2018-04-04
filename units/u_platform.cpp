#include "stdafx.h"

#include "u_platform.h"

///*static*/ constexpr platform::type platform::get(
//) {
//#if defined(_WIN64)
//	return win64;
//#elif defined(_WIN32)
//	return win32;
//#else
//	return unknown;
//#endif
//}
//
///*static*/ constexpr bool platform::is_windows(
//) {
//	switch ( platform::get() )
//	{
//		case win64:
//		case win32:
//			return true;
//		default:
//			return false;
//	}
//}