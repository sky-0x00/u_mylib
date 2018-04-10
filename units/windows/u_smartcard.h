#pragma once

#ifdef _WIN32
	#include <winscard.h>
#else
	#define SCARD_S_SUCCESS		0

	#define SCARD_SCOPE_USER	0
	#define SCARD_SCOPE_SYSTEM	2
#endif

#include "..\u_macros.h"
#include "..\u_string.h"

class smartcard
{
public:
	typename SCARDHANDLE handle;

	class context
	{
	public:
		typedef SCARDCONTEXT data, *pdata;

		enum class scope
		{
			user = SCARD_SCOPE_USER,
			system = SCARD_SCOPE_SYSTEM
		};

	public:
		context( _in scope scope );
		~context();
		operator data() const;

	private:
		static long establish( _in scope scope, _out data &data ) noexcept;
		static long release( _in const data &data ) noexcept;

	private:
		data m_data;
	};

	class memory
	{
	public:
		memory( _in context context );
		memory& operator()( _in cpvoid_t pmemory );
	private:
		context &m_context;
	};

	class manager
	{
	public:
		manager( /*_in const context &context*/ );
		string::list enum_devices() const;

	private:
		static long enum_devices( _in const context &context, _out string::list &devices ) noexcept;

		context m_context;
	};

public:
private:
};
