#pragma once

#include "u_macros.h"

// статический ассерт (ассерт времени компиляции)
#define assert_static( expr )		static_assert( expr, #expr )

// динамический ассерт (ассерт времени выполнения)
void assert_dynamic( _in bool expr );

// макрос с коротким именем
#ifdef assert
	#undef assert
#endif
#define assert( expr )				assert_dynamic( expr )
