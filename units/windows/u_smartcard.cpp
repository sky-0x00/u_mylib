#include "stdafx.h"
#include "u_smartcard.h"

#include "winapi\u_winapi.h"
#include "..\u_assert.h"
#include <memory>

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _WIN32
#else
	#define SCARD_AUTOALLOCATE		(DWORD)(-1)
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------
smartcard::memory::memory( 
	_in context context 
) :
	m_context( context )
{}
smartcard::memory& smartcard::memory::operator()( 
	_in cpvoid_t pmemory 
) {
	if ( pmemory )
		Winapi::SCardFreeMemory( m_context, pmemory );
	return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
smartcard::context::context(
	_in scope scope
) :
	m_data(0)
{
	const auto result = establish( scope, m_data );
	assert( SCARD_S_SUCCESS == result, result );
}
smartcard::context::~context(
) {
	/*const auto result =*/ release( m_data );
	//assert( SCARD_S_SUCCESS == result );
}

smartcard::context::operator data(
) const
{
	return m_data;
}

/*static*/ long smartcard::context::establish( 
	_in scope scope, 
	_out data &data 
) noexcept 
{
	return Winapi::SCardEstablishContext( static_cast<DWORD>(scope), &data );
}
/*static*/ long smartcard::context::release( 
	_in const data &data
) noexcept
{
	return Winapi::SCardReleaseContext( data );
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
smartcard::manager::manager( 
	//_in const context &context 
) :
	m_context( context::scope::user )
{}

/*static*/ long smartcard::manager::enum_devices( 
	_in const context &context,
	_out string::list &devices
) noexcept
{
	//devices.clear();

	LPWSTR mszDevices = nullptr;
	DWORD CharsCount = SCARD_AUTOALLOCATE;

	const auto Result = Winapi::SCardListReaders( context, nullptr, reinterpret_cast<LPWSTR>(&mszDevices), &CharsCount );
	
	// нет ридеров, выходим
	if (SCARD_E_NO_READERS_AVAILABLE == Result)
		return SCARD_S_SUCCESS;

	// что-то сломалось
	if (SCARD_S_SUCCESS != Result)
		return Result;

	// все хорошо, парсим мультистроку, но сначала установим сайф-клинер автоаллоцированного блока mszDevices
	struct autoclose
	{
		typedef std::unique_ptr<void, memory> memory;
	};
	autoclose::memory sp_devices( mszDevices, context );
	string device;

	for ( decltype(CharsCount) i = 0; i < CharsCount; ++i )
	{
		const auto ch = mszDevices[i];
		if ( ch )
		{
			device.push_back(ch);
			continue;
		}
		if ( !device.empty() )
		{
			devices.push_back( device );
			device.clear();
		}
	}
	return Result;
}

string::list smartcard::manager::enum_devices(
) const
{
	string::list devices;
	const auto result = enum_devices( m_context, devices );
	assert( SCARD_S_SUCCESS == result, result );
	return devices;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------