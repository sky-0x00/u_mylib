#include "stdafx.h"

#include "u_winapi.h"

Winapi::Error Winapi::GetLastError(
) noexcept
{
	return static_cast<Error>( ::GetLastError() );
}
void Winapi::SetLastError( 
	_in Error Error 
) noexcept
{
	::SetLastError( static_cast<DWORD>(Error) );
}