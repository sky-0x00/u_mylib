#pragma once

#include "..\u_system.h"
#include "..\u_macros.h"
#include <array>

namespace network 
{
	namespace win32
	{
		class library
		{
		public:
			class error
			{
			public:
				typedef int code;
				error( _in code code = 0 );				// 0 - нет ошибок

				static error get_last();
				operator int() const;

			private:
				code m_code;
			};
			typedef std::array< unsigned, 2 > version;

			library( _in const version &version = {2, 2} );
			~library();

		private:
			static error startup( _in const version &version ) noexcept;
			static error cleanup() noexcept;
		};
	}

#ifdef _WIN32
	typedef win32::library library;
#else
	#include "..\u_platform.h"
	#error PLATFORM_UNSUPPORTED
#endif
}