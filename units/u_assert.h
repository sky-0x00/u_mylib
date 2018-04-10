#pragma once

#include "u_macros.h"
#include "windows\winapi\u_winapi.h"

// статический ассерт (ассерт времени компиляции)
#define assert_static( expr )		static_assert( expr, #expr )

// динамический ассерт (ассерт времени выполнения)
void assert_dynamic( _in bool expr, _in Winapi::Error error = Winapi::GetLastError() );

// макрос с коротким именем
#ifdef assert
	#undef assert
#endif
#define assert( expr, error )				assert_dynamic( expr, error )
