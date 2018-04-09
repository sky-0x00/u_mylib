#include "stdafx.h"
#include "u_winapi.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------
int Winapi::WSAStartup( 
	_in WORD VersionRequested, 
	_out _option LPWSADATA pWSAData /*= nullptr*/
) noexcept
{
	WSADATA WSAData;
	if ( !pWSAData )
		pWSAData = &WSAData;
	int Result = ::WSAStartup( VersionRequested, pWSAData );
	//if ( 0 == Result )
		// trace normal
	//else
		// trace error
	return Result;
}
int Winapi::WSACleanup(
) noexcept
{
	int Result = ::WSACleanup();
	//if ( 0 == Result )
	// trace normal
	//else
	// trace error
	return Result;
}
int Winapi::WSAGetLastError(
) noexcept
{
	return ::WSAGetLastError();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------