#pragma once

#include "..\u_types.h"
#include "..\..\u_macros.h"

namespace Winapi
{
#ifdef SCardListReaders
	#undef SCardListReaders
#endif

	typedef module_t Module;
	typedef handle_t Handle;
	typedef unsigned Error;

	//enum class Error
	//{
	//	Success = ERROR_SUCCESS,						/*0*/
	//	InvalidFunction = ERROR_INVALID_FUNCTION,		/*1*/
	//	FileNotFound = ERROR_FILE_NOT_FOUND,			/*2*/
	//	PathNotFound = ERROR_PATH_NOT_FOUND,			/*3*/
	//	TooManyOpenFiles = ERROR_TOO_MANY_OPEN_FILES,	/*4*/
	//	AccessDenied = ERROR_ACCESS_DENIED,				/*5*/
	//	InvalidParameter = ERROR_INVALID_PARAMETER,		/*87*/
	//	BadArguments = ERROR_BAD_ARGUMENTS,				/*160*/
	//	AssertionFailure = ERROR_ASSERTION_FAILURE,		/*668*/
	//	AcpiError = ERROR_ACPI_ERROR,					/*669*/
	//	WowAssertion = ERROR_WOW_ASSERTION              /*670*/
	//};
	
	Error GetLastError() noexcept;
	void SetLastError( _in Error Error ) noexcept;

	int WSAStartup( _in WORD VersionRequested, _out _option LPWSADATA pWSAData = nullptr ) noexcept;
	int WSACleanup() noexcept;
	int WSAGetLastError() noexcept;

	long SCardEstablishContext( _in DWORD Scope, _out LPSCARDCONTEXT pContext ) noexcept;
	long SCardReleaseContext( _in SCARDCONTEXT Context ) noexcept;
	long SCardListReaders( _in SCARDCONTEXT Context, _in _option LPCWSTR mszGroups, _out LPWSTR mszDevices, _in _option LPDWORD pcchDevices ) noexcept;
	long SCardFreeMemory( _in SCARDCONTEXT Context, _in LPCVOID pMemory ) noexcept;
}
