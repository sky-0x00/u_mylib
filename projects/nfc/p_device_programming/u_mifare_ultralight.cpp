#include "stdafx.h"
#include "u_mifare_ultralight.h"

#include <sstream>
#include <iomanip>
#include <windows.h>
#include <assert.h>
#include <chrono>

#define SW1_OK		0x90
#define SW1_ERROR	0x63
#define SW2			0x00				// одинаков в случае успеха и неудачи

#define TRACE_SCARD_ERROR(error)		CSmartcardError(error, __FUNCTIONW__)


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
cardtype_e get_cardtype(size_t pagecount)
{
	switch (static_cast<cardtype_e>(pagecount))
	{
		case cardtype_e::ultralight:
			return cardtype_e::ultralight;
		case cardtype_e::ultralight_ev1_20p:
			return cardtype_e::ultralight_ev1_20p;
		case cardtype_e::ultralight_ev1_41p:
			return cardtype_e::ultralight_ev1_41p;
		default:
			return cardtype_e::unknown;
	}
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool apdu::packet_t::is_sw_ok(const byte_t sw[2])
{
	if (SW2 == sw[1])
	{
		if (SW1_OK == sw[0]) return true;
		if (SW1_ERROR == sw[0]) return false;
	}

	throw apdu::error::sw_t(__FUNCTIONW__, sw);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
apdu::error::out_of_range_t::out_of_range_t(
	_In_ const wchar_t *function,
	_In_ size_t value,
	_In_ size_t expected_value
) : error_t(function) 
{
	std::wostringstream stream;
	stream << L"value \"" << value << L"\", expected value: \"" << expected_value << L"\"";
	output_message(stream.str().c_str());
}
apdu::error::out_of_range_t::out_of_range_t(
	_In_ const wchar_t *function,
	_In_ size_t value,
	_In_ const size_t bounds[2]
) : error_t(function)
{
	std::wostringstream stream;
	stream << L"value \"" << value << L"\" out of range [" << bounds[0] << L"; " << bounds[1] << L"]";
	output_message(stream.str().c_str());
}

apdu::error::sw_t::sw_t(
	_In_ const wchar_t *function, 
	_In_ const byte_t sw[2]
) : error_t (function)
{
	std::wostringstream stream;
	stream << L"sw1: " << std::showbase << std::internal << std::hex << std::setfill(L'0') << std::setw(4) << sw[0] <<
		L", sw2: " << std::showbase << std::internal << std::hex << std::setfill(L'0') << std::setw(4) << sw[1];
	output_message(stream.str().c_str());
}

apdu::error::bytes_count_not_equal_buffer_size_t::bytes_count_not_equal_buffer_size_t(
	_In_ const wchar_t *function,
	_In_ size_t buffer_size,
	_In_ size_t bytes_count
) : error_t(function)
{
	std::wostringstream stream;
	stream << L"buffer_size: " << buffer_size << L", bytes_count: " << bytes_count;
	output_message(stream.str().c_str());
}

apdu::error::error_t::error_t(
	_In_ const wchar_t *function
) : m_function (function) {}

void apdu::error::error_t::output_message(_In_ const wchar_t *message) const
{
	std::wostringstream stream;
	stream << L"exp> func: " << m_function << L"(), error: " << message << std::endl;
	::OutputDebugString(stream.str().c_str());
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
APDU_Manager::APDU_Manager(_In_ const CSmartcardManager &SmartcardManager)
	: m_SmartcardManager(SmartcardManager) 
{
	m_packet.send.reserve(5);
	m_packet.send.push_back(0xFF);		// class
	
	m_packet.recieve.reserve(MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE + 2);
}

array_t<byte_t> APDU_Manager::Read_BinaryBlock(
	_In_ size_t Page,
	_In_ size_t BytesToRead
) const
{
	array_t<byte_t> result;

	BytesToRead = GetBytesToRead(Page, BytesToRead);

	m_packet.send.resize(5);
	m_packet.send[1] = 0xB0;								// INS
	m_packet.send[2] = 0x00;								// P1
	m_packet.send[3] = static_cast<byte_t>(Page);			// P2
	m_packet.send[4] = static_cast<byte_t>(BytesToRead);	// Le

	m_packet.recieve.resize(2 + BytesToRead);

	const SCARD_IO_REQUEST Request = { m_SmartcardManager.GetProtocol(), sizeof(SCARD_IO_REQUEST) };
	ULONG BytesReaded = m_packet.recieve.size();

	auto error = m_SmartcardManager.Transmit(&Request, m_packet.send.data(), m_packet.send.size(), nullptr, m_packet.recieve.data(), &BytesReaded);
	
	if (SCARD_S_SUCCESS != error)
		TRACE_SCARD_ERROR(error);

	if (BytesReaded != m_packet.recieve.size())
		throw apdu::error::bytes_count_not_equal_buffer_size_t(__FUNCTIONW__, m_packet.recieve.size(), BytesReaded);

	if (apdu::packet_t::is_sw_ok(BytesToRead + m_packet.recieve.data()))
	{
		result.data = m_packet.recieve.data();
		result.size = BytesToRead;
	}

	return result;
}

size_t APDU_Manager::GetBytesToRead(
	_In_ size_t Page,
	_In_ size_t BytesToRead
) const 
{
	// имеется всего 16 страниц памяти - [0..15]
	//if (Page >= MIFARE_ULTRALIGHT__PAGE_COUNT)
	//	throw apdu::error::out_of_range_t(__FUNCTIONW__, BytesToRead, (0, MIFARE_ULTRALIGHT__PAGE_COUNT - 1) );

	//// читать разрешается не более 16 байт за раз
	//if (BytesToRead > MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE)
	//	throw apdu::error::out_of_range_t(__FUNCTIONW__, BytesToRead, (0, MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE) );

	//if (MIFARE_ULTRALIGHT__BYTES_PER_PAGE * Page + BytesToRead <= MIFARE_ULTRALIGHT__SIZE_IN_BYTES)
		return BytesToRead;

	return MIFARE_ULTRALIGHT__SIZE_IN_BYTES - MIFARE_ULTRALIGHT__BYTES_PER_PAGE * Page - BytesToRead % MIFARE_ULTRALIGHT__BYTES_PER_PAGE;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef unsigned short crc_t;

crc_t crc(const array_t<byte_t> &data)
{
	crc_t result = 0x6363;
	return 0;
}

array_t<byte_t> APDU_Manager::GetVersion() const
{
	array_t<byte_t> result;

	m_packet.send.resize(6);
	m_packet.send[1] = 0x00;								// INS - pseudo apdu
	m_packet.send[2] = 0x00;								// P1
	m_packet.send[3] = 0x00;								// P2
	m_packet.send[4] = 0x01;								// Lc	
	m_packet.send[5] = 0x60;								// payload: cmd="GET_VERSION"
	//m_packet.send[6] = 0x00;								// payload: cmd="GET_VERSION"
	
	m_packet.recieve.resize(8);

	const SCARD_IO_REQUEST Request = { m_SmartcardManager.GetProtocol(), sizeof(SCARD_IO_REQUEST) };
	ULONG BytesReaded = m_packet.recieve.size();

	SCARD_IO_REQUEST resp = { m_SmartcardManager.GetProtocol(), sizeof(SCARD_IO_REQUEST) };

	auto error = m_SmartcardManager.Transmit(&Request, m_packet.send.data(), m_packet.send.size(), &resp, m_packet.recieve.data(), &BytesReaded);

	if (SCARD_S_SUCCESS != error)
		TRACE_SCARD_ERROR(error);

	/*if (BytesReaded != m_packet.recieve.size())
		throw apdu::error::bytes_count_not_equal_buffer_size_t(__FUNCTIONW__, m_packet.recieve.size(), BytesReaded);

	if (apdu::packet_t::is_sw_ok(8 + m_packet.recieve.data()))
	{
		result.data = m_packet.recieve.data();
		result.size = 8;
	}*/

	m_packet.send.resize(5);
	m_packet.send[1] = 0xC0;								// INS - pseudo apdu
	m_packet.send[2] = 0x00;								// P1
	m_packet.send[3] = 0x00;								// P2
	m_packet.send[4] = 0x08;								// Lc	

	m_packet.recieve.resize(10);

	BytesReaded = m_packet.recieve.size();

	error = m_SmartcardManager.Transmit(&Request, m_packet.send.data(), m_packet.send.size(), nullptr, m_packet.recieve.data(), &BytesReaded);

	if (SCARD_S_SUCCESS != error)
		TRACE_SCARD_ERROR(error);

	//if (BytesReaded != m_packet.recieve.size())
	//	throw apdu::error::bytes_count_not_equal_buffer_size_t(__FUNCTIONW__, m_packet.recieve.size(), BytesReaded);

	//if (apdu::packet_t::is_sw_ok(8 + m_packet.recieve.data()))
	//{
	//	result.data = m_packet.recieve.data();
	//	result.size = 8;
	//}



	return result;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void mifare_ultralight::format::human::get( 
	IN const std::vector< byte_t > &dump_raw
) {
	auto data_raw = reinterpret_cast< const format::raw * >( dump_raw.data() );

	// заполняем данными в порядке следования полей в билете
	get_uid( data_raw->uid_begin );

	bcc[0] = data_raw->bcc_0;
	bcc[1] = data_raw->bcc_1;
	assert( check_bcc() );

	lock = _byteswap_ushort( data_raw->lock );
	otp = _byteswap_ulong( data_raw->otp );

	get_serial( reinterpret_cast< cdata_t >( &data_raw->serial_begin ) );

	assert( data_raw->date_validto_0 == data_raw->date_validto_1 );
	assert( data_raw->time_validto_0 == data_raw->time_validto_1 );
	init_and_decode_tm( tm_validto, &data_raw->date_validto_0, &data_raw->time_validto_0 );
	init_and_decode_tm( tm_dateofsale, &data_raw->date_ofsale, nullptr );
	validity = data_raw->validity;
	counter = data_raw->counter;
	wicket_no = data_raw->wicket_no;
	hash = data_raw->hash;
	init_and_decode_tm( tm_lastuse, &data_raw->date_of_lastuse, &data_raw->time_of_lastuse );
}

/*static*/ void mifare_ultralight::format::human::get_uid( 
	IN cdata_t uid_begin, 
	OUT byte_t (&uid)[ MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES ] 
) {
	uid[0] = uid_begin[0];
	uid[1] = uid_begin[1];
	uid[2] = uid_begin[2];
	uid[3] = uid_begin[4];
	uid[4] = uid_begin[5];
	uid[5] = uid_begin[6];
	uid[6] = uid_begin[7];
}

void mifare_ultralight::format::human::get_uid( 
	IN cdata_t uid_begin
) {
	get_uid( uid_begin, uid );
}

bool mifare_ultralight::format::human::check_bcc( 
) const 
{
	byte_t bcc_calc[2];
	bcc_calc[0] = 0x88 ^ uid[0] ^ uid[1] ^ uid[2];
	bcc_calc[1] = uid[3] ^ uid[4] ^ uid[5] ^ uid[6];
	return (bcc[0] == bcc_calc[0]) && (bcc[1] == bcc_calc[1]);
}

void mifare_ultralight::format::human::get_serial( 
	IN cdata_t serial_begin
) {
	const auto p_value = reinterpret_cast< const uint32_t * >( serial_begin );
	serial = ( _byteswap_ulong( p_value[0] ) & 0xFFF) << 0x14;
	serial |= ( _byteswap_ulong( p_value[1] ) >> 0xC) & 0xFFFFF;
}

namespace std
{
	namespace chrono
	{
		typedef duration< uint32_t, ratio_multiply< ratio<24>, hours::period> > days;				// расширим std::chrono типом duration длинною в день
	}
}

/*static*/ void mifare_ultralight::format::human::decode_date( 
	IN uint16_t encoded, 
	OUT std::tm &decoded 
) {
	// дата представлена как число дней, прошедших с 1992-01-01 (среда)
	
	auto get_epoch = []() -> time_t
	{
		std::tm tm_epoch = { 
			0,			// tm_sec
			0,			// tm_min
			0,			// tm_hour
			0,			// tm_mday [1..31]	// ?
			0,			// tm_mon [0..11]
			92,			// tm_year [since 1900]
			3,			// tm_wday - days since Sunday - [0..6]
			0,			// tm_yday - days since January 1 - [0..365]
			0			// tm_isdst - daylight savings time flag
		};

		//const auto s = asctime( &tm_epoch );

		const auto result = std::mktime( &tm_epoch );
		assert( -1 != result );
		return result;
	};
	
	static const auto tp_epoch = std::chrono::steady_clock::from_time_t( get_epoch() );							// epoch
	const auto t_new = std::chrono::steady_clock::to_time_t( tp_epoch + std::chrono::days( encoded ) );			// epoch + encoded
	decoded = *std::localtime( &t_new );

	//const auto t_new = get_epoch();	
	//decoded = *std::localtime( &t_new );

	const auto s = asctime( &decoded );
}

/*static*/ void mifare_ultralight::format::human::decode_time( 
	IN uint16_t encoded, 
	OUT std::tm &decoded 
) {
	// 0x6728 = 26408 = 26408/32 = 825 минут от 00:00
	const double mins = encoded / 32.0;	
	const auto mins_rounded = static_cast< uint16_t >( mins );

	decoded.tm_hour = mins_rounded / 60;
	decoded.tm_min = mins_rounded % 60;
	decoded.tm_sec = static_cast< int >( 60 * (mins - mins_rounded) );
}

/*static*/ void mifare_ultralight::format::human::init_and_decode_tm( 
	OUT std::tm &decoded,
	IN const uint16_t *encoded_date, 
	IN const uint16_t *encoded_time
) {
	decoded = {};
	if (encoded_date)
		decode_date( _byteswap_ushort( *encoded_date ), decoded );
	if (encoded_time)
		decode_time( _byteswap_ushort( *encoded_time ), decoded );
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------