#include "stdafx.h"
#include "u_network.h"

#include "..\windows\winapi\u_winapi.h"
#include "..\u_assert.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _WIN32
	#pragma comment (lib, "ws2_32.lib")
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------
network::win32::library::error::error(
	_in code code /*= 0*/
) :
	m_code( code )
{}

network::win32::library::error::operator int(
) const
{
	return m_code;
}

/*static*/ network::win32::library::error network::win32::library::error::get_last(
) {
	return Winapi::WSAGetLastError();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ network::win32::library::error network::win32::library::startup(
	_in const version &version
) noexcept 
{
	return Winapi::WSAStartup( MAKEWORD(version.front(), version.back()) );
}
/*static*/ network::win32::library::error network::win32::library::cleanup(
) noexcept 
{
	return Winapi::WSACleanup();
}

network::win32::library::library( 
	_in const version &version /*= { 2, 2 }*/
) {
	const auto result = startup( version );
	assert( 0 == result, result );
}
network::win32::library::~library(
) {
	/*const auto Result =*/ cleanup();
	//assert( 0 == Result );
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------