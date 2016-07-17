#include "stdafx.h"
#include "u_mifare_ultralight.h"

#include <sstream>
#include <iomanip>
#include <windows.h>

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
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------