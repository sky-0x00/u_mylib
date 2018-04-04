#pragma once

#include "u_macros.h"

// ����������� ������ (������ ������� ����������)
#define assert_static( expr )		static_assert( expr, #expr )

// ������������ ������ (������ ������� ����������)
void assert_dynamic( _in bool expr );

// ������ � �������� ������
#ifdef assert
	#undef assert
#endif
#define assert( expr )				assert_dynamic( expr )
