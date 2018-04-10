#include "stdafx.h"

#include "u_assert.h"
#include "u_platform.h"
#include "windows\winapi\u_winapi.h"

void assert_dynamic(
	_in bool expr,
	_in Winapi::Error error /*= Winapi::GetLastError()*/
) {
	if (expr)
		return;

	//if ( platform::is_windows() )
	//{
		// trace что-либо...
		throw error;
	//}

	// неподдерживаемая платформа
	//throw PLATFORM_UNSUPPORTED;
}