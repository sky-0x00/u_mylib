#include "stdafx.h"
#include "u_winapi.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Winapi::Error Winapi::GetLastError(
) noexcept
{
	return ::GetLastError();
}
void Winapi::SetLastError( 
	_in Error Error 
) noexcept
{
	::SetLastError(Error);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma comment( lib, "ws2_32.lib" )

int Winapi::WSAStartup( 
	_in WORD VersionRequested, 
	_out _option LPWSADATA pWSAData /*= nullptr*/
) noexcept
{
	WSADATA WSAData;
	if ( !pWSAData )
		pWSAData = &WSAData;
	const auto Result = ::WSAStartup( VersionRequested, pWSAData );
	//if ( 0 == Result )
		// trace normal
	//else
		// trace error
	return Result;
}
int Winapi::WSACleanup(
) noexcept
{
	const auto Result = ::WSACleanup();
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
#pragma comment( lib, "winscard.lib" )

long Winapi::SCardEstablishContext( 
	_in DWORD Scope, 
	_out LPSCARDCONTEXT pContext 
) noexcept
{
	const auto Result = ::SCardEstablishContext( Scope, nullptr, nullptr, pContext );
	//if ( SCARD_S_SUCCESS == Result )
	// trace normal
	//else
	// trace error
	return Result;
}
long Winapi::SCardReleaseContext( 
	_in SCARDCONTEXT Context 
) noexcept 
{
	const auto Result = ::SCardReleaseContext( Context );
	//if ( SCARD_S_SUCCESS == Result )
	// trace normal
	//else
	// trace error
	return Result;
}

long Winapi::SCardListReaders( 
	_in SCARDCONTEXT Context, 
	_in _option LPCWSTR mszGroups, 
	_out LPWSTR mszReaders, 
	_in _option LPDWORD pcchReaders 
) noexcept 
{
	const auto Result = ::SCardListReadersW( Context, mszGroups, mszReaders, pcchReaders );
	//if ( SCARD_S_SUCCESS == Result )
	// trace normal
	//else
	// trace error
	return Result;
}

long Winapi::SCardFreeMemory( 
	_in SCARDCONTEXT Context, 
	_in LPCVOID pMemory 
) noexcept
{
	const auto Result = ::SCardFreeMemory( Context, pMemory );
	//if ( SCARD_S_SUCCESS == Result )
	// trace normal
	//else
	// trace error
	return Result;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------