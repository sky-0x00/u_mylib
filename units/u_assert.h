#pragma once

#include "u_macros.h"
#include "windows\winapi\u_winapi.h"

// ����������� ������ (������ ������� ����������)
#define assert_static( expr )		static_assert( expr, #expr )

// ������������ ������ (������ ������� ����������)
void assert_dynamic( _in bool expr, _in Winapi::Error error = Winapi::GetLastError() );

// ������ � �������� ������
#ifdef assert
	#undef assert
#endif
#define assert( expr, error )				assert_dynamic( expr, error )
