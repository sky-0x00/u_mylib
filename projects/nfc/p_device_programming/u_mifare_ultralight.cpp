#include "stdafx.h"
#include "u_mifare_ultralight.h"

#include <sstream>
#include <iomanip>
#include <windows.h>

#define SW1_OK		0x90
#define SW1_ERROR	0x63
#define SW2			0x00		// одинаков в случае успеха и неудачи

#define TRACE_SCARD_ERROR(error)		CSmartcardError(error, __FUNCTIONW__)

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
apdu::receive_t::receive_t()
	: packet(2) {}

bool apdu::receive_t::is_ok() const
{
	auto p_sw = get_sw();
	if (SW2 == p_sw[1])
	{
		if (SW1_OK == p_sw[0]) return true;
		if (SW1_ERROR == p_sw[0]) return false;
	}
	throw error::sw_t(__FUNCTIONW__, p_sw);
}

inline ULONG apdu::receive_t::get_size() const
{
	return 2 + packet.size();
}

const byte_t* apdu::receive_t::get_sw() const noexcept
{
	return packet.data() + packet.size() - 2;
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
apdu::send_t::send_t(_In_ byte_t cls, _In_ byte_t ins)
	: packet({ cls, ins, 0, 0, 0 }) {}

inline ULONG apdu::send_t::get_size() const
{
	return 5;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
apdu::error::out_of_range_t::out_of_range_t(
	_In_ const wchar_t *function,
	_In_ size_t value,
	_In_ size_t upper_bound
) : error_t(function) 
{
	const size_t bounds[] = { 0, upper_bound };
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
APDU::APDU(_In_ const CSmartcardManager &SmartcardManager)
	: m_SmartcardManager(SmartcardManager) {}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
APDU_ReadData::APDU_ReadData(_In_ const CSmartcardManager &SmartcardManager)
	: APDU(SmartcardManager), m_send(0xFF, 0xB0)
{
	m_receive.packet.reserve(MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE);
}

byte_t APDU_ReadData::GetBytesToRead(
	_In_ byte_t Page, 
	_In_ byte_t BytesToRead
) const 
{
	// имеется всего 16 страниц памяти - [0..15]
	if (Page >= MIFARE_ULTRALIGHT__PAGE_COUNT)
		throw apdu::error::out_of_range_t(__FUNCTIONW__, BytesToRead, MIFARE_ULTRALIGHT__PAGE_COUNT - 1);

	// читать разрешается не более 16 байт за раз
	if (BytesToRead > MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE)
		throw apdu::error::out_of_range_t(__FUNCTIONW__, BytesToRead, MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE);

	if (MIFARE_ULTRALIGHT__BYTES_PER_PAGE * Page + BytesToRead <= MIFARE_ULTRALIGHT__SIZE_IN_BYTES)
		return BytesToRead;

	return MIFARE_ULTRALIGHT__SIZE_IN_BYTES - MIFARE_ULTRALIGHT__BYTES_PER_PAGE * Page - BytesToRead % MIFARE_ULTRALIGHT__BYTES_PER_PAGE;
}

ULONG APDU_ReadData::Execute(
	_In_ byte_t Page, 
	_In_ byte_t BytesToRead
) noexcept
{
	BytesToRead = GetBytesToRead(Page, BytesToRead);
	
	// читаем первые BytesToRead байтов, начиная со страницы Page
	m_send.packet.p[1] = Page;
	m_send.packet.p[2] = BytesToRead;

	// ожидаем (2 + BytesToRead) байтов получить в ответ - данный + ответ sw
	ULONG BytesReaded = 2 + BytesToRead;
	m_receive.packet.resize(BytesReaded);

	const SCARD_IO_REQUEST Request = { m_SmartcardManager.GetProtocol(), sizeof(SCARD_IO_REQUEST) };	
	auto error = m_SmartcardManager.Transmit(&Request, reinterpret_cast<LPCBYTE>(&m_send.packet), m_send.get_size(), nullptr, m_receive.packet.data(), &BytesReaded);
	if (SCARD_S_SUCCESS != error)
		TRACE_SCARD_ERROR(error);
	
	// также должно быть выполнено (BytesReaded == mp_receive->packet.size()), т.е. прочиталось столько байт, сколько выделили памяти
	//assert(BytesReaded == mp_receive->get_size());
	
	return error;
}

const apdu::content_t* APDU_ReadData::GetData() const noexcept
{
	return (2 != m_receive.packet.size()) && m_receive.is_ok() ? &m_receive.packet : nullptr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------