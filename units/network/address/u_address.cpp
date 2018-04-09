#include "stdafx.h"

#include "u_address.h"
#include "..\..\u_assert.h"

void network::address::protocol::ip_v4::set(
	_in const octets &ip
) {
	m_data = ip;
}

byte_t& network::address::protocol::ip_v4::octet(
	_in unsigned index
) {
	assert( index < std::tuple_size<octets>::value );
	return m_data[index];
}
//byte_t protocol::ip_v4::get_octet( 
//	_in unsigned index 
//) const
//{
//	assert( index < std::tuple_size<octets>::value );
//	return m_data[index];
//}
//void protocol::ip_v4::set_octet( 
//	_in unsigned index, 
//	_in byte_t value 
//) {
//	assert( index < std::tuple_size<octets>::value );
//	m_data[index] = value;
//}

network::address::protocol::ip_v4::ip_v4(
	_in const octets &ip /*= { 0, 0, 0, 0 } */
) {
	set(ip);
}
network::address::protocol::ip_v4::ip_v4(
	_in const ip_v4 &ip
) {
	set( ip.m_data );
}

network::address::protocol::ip_v4& network::address::protocol::ip_v4::operator =(
	_in const octets &ip 
) {
	set(ip);
	return *this;
}
network::address::protocol::ip_v4& network::address::protocol::ip_v4::operator =(
	_in const ip_v4 &ip 
) {
	set(ip.m_data);
	return *this;
}

string network::address::protocol::ip_v4::to_string(
) const
{
	auto octet_to_string = [](
		_in byte_t octet,
		_out char_t (&buffer)[4]
	) -> cstr_t
	{
		const auto error = _ultow_s( octet, buffer, 10 );				// TODO: убрать зависимость от ucrt
		assert( 0 == error );
		return buffer;
	};

	char_t buffer[4];
	unsigned i = 0;
	string result = octet_to_string( m_data.at(i++), buffer );
	while ( i < std::tuple_size<octets>::value )
	{
		result.push_back( L'.' );
		result += octet_to_string( m_data.at(i++), buffer );
	}
	return result;
}

string network::address::to_string(
	_in bool is_output_port /*= true*/
) const
{
	string result( m_host.to_string() );
	if ( is_output_port )
	{
		char_t buffer[6];
		const auto error = _ultow_s( m_port, buffer, 10 );				// TODO: убрать зависимость от ucrt
		assert( 0 == error );

		result.push_back( L':' );
		result += buffer;
	}
	return result;
}