#pragma once

#include "..\..\u_system.h"
#include "..\..\u_macros.h"
#include "..\..\u_string.h"
#include <array>

namespace network
{
	namespace address
	{
		namespace protocol
		{
			class ip_v4
			{
			public:
				typedef std::array<byte_t, 4> octets;
			
			public:
				ip_v4( _in const octets &ip = {0, 0, 0, 0} );
				ip_v4( _in const ip_v4 &ip );

				ip_v4& operator =( _in const octets &ip );
				ip_v4& operator =( _in const ip_v4 &ip );

				byte_t& octet( _in unsigned index );

				string to_string() const;

			private:				
				void set( _in const octets &ip );

				//byte_t get_octet( _in unsigned index ) const;
				//void set_octet( _in unsigned index, _in byte_t value );

				octets m_data;
			};

			class ip_v6
			{};

			typedef ip_v4 ip;
		}

		typedef uint16_t port;
		typedef protocol::ip host;
	}
}
