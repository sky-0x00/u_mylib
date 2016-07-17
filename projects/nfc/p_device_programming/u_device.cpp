#include "stdafx.h"
#include "u_device.h"

#include "u_scard_wrapper.h"
#include <fstream>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define THROW_SCARD_ERROR(error)		throw CSmartcardError(error, __FUNCTIONW__)

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace memory
{
	// копирование непересекающихся блоков памяти, кратных 4 байтам
	void copy__f(void* dest, const void* src, size_t bytes_count)
	{
		auto p_dest = static_cast<unsigned __int32 *>(dest);
		auto p_src = static_cast<const unsigned __int32 *>(src);
		
		for (bytes_count >>= 2; bytes_count--; ++p_dest, ++p_src)
			*p_dest = *p_src;
	}
}

namespace smartcard
{
	enum class protocol 
	{
		t0 = SCARD_PROTOCOL_T0,
		t1 = SCARD_PROTOCOL_T1
	};
	enum class sharemode
	{
		shared = SCARD_SHARE_SHARED,
		exclusive = SCARD_SHARE_EXCLUSIVE,
		direct = SCARD_SHARE_DIRECT
	};
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
smartcard::device_t::device_t()
	: m_manager(m_context), m_apdu(m_manager), m_pagecount(0) {}

string_list_t smartcard::device_t::enum_all() const
{
	return m_manager.EnumDevices();
}

size_t smartcard::device_t::determine_pagecount()
{
	m_pagecount = 0;
	try
	{
		for (;; ++m_pagecount)
			read_data(m_pagecount, MIFARE_ULTRALIGHT__BYTES_PER_PAGE);
	}
	catch (...)
	{
		return m_pagecount;
	}
}

void smartcard::device_t::connect(
	_In_ const wchar_t *name
)
{
	auto error = m_manager.Connect(
		name, 
		static_cast<ULONG>(smartcard::sharemode::shared), 
		static_cast<ULONG>(smartcard::protocol::t0) | static_cast<ULONG>(smartcard::protocol::t1)
	);

	if (SCARD_S_SUCCESS != error)
		THROW_SCARD_ERROR(error);
}

array_t<byte_t> smartcard::device_t::read_data(
	_In_ size_t page,
	_In_ size_t bytes_count
) const
{
	return m_apdu.Read_BinaryBlock(page, bytes_count);
}

size_t smartcard::device_t::dump(
	_Out_ std::vector<byte_t> &buffer
) const noexcept
{
	buffer.clear();
	size_t pagecount = 0;

	try
	{
		// читаем страницы по 4 байта, выход - по ошибке чтения
		for (;; ++pagecount)
		{
			auto BinaryBlock = m_apdu.Read_BinaryBlock(pagecount, 4);
			if (4 == BinaryBlock.size)
				buffer.insert(buffer.end(), BinaryBlock.data, BinaryBlock.data + 4);
		}
	}
	catch (...)
	{
		return pagecount;
	}
}
//
//void smartcard::device_t::dump(
//	_Out_ byte_t buffer[MIFARE_ULTRALIGHT__SIZE_IN_BYTES]
//) const
//{
//	dump_internal(buffer);
//}
//
////
//// true: успех, в буфер записано MIFARE_ULTRALIGHT__SIZE_IN_BYTES байт
//// false: размер буфера недостаточен (значение buffer_size меньше MIFARE_ULTRALIGHT__SIZE_IN_BYTES)
////
//bool smartcard::device_t::dump(
//	_Out_ byte_t *p_buffer, 
//	_In_ size_t buffer_size
//) const
//{
//	if (buffer_size < MIFARE_ULTRALIGHT__SIZE_IN_BYTES)
//		return false;
//
//	dump_internal(p_buffer);
//	return true;
//}
//
//
//void smartcard::device_t::dump(_In_ const wchar_t *file_name) const
//{
//	std::ofstream f_out(file_name, std::wofstream::binary);
//
//	for (size_t i = 0; i < (MIFARE_ULTRALIGHT__SIZE_IN_BYTES / MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE); ++i)
//	{
//		auto BinaryBlock = m_apdu.Read_BinaryBlock(i, MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE);
//		f_out.write(reinterpret_cast<const char*>(BinaryBlock.data), BinaryBlock.size);
//	}
//
//	f_out.close();
//}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void smartcard::device_t::dump_sn(
	_Out_ byte_t buffer[MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES]
) const
{
	dump_sn_internal(buffer);
}

//
// true: успех, в буфер записано MIFARE_ULTRALIGHT__SERIAL_NUMBER_SIZE_IN_BYTES байт
// false: размер буфера недостаточен (значение buffer_size меньше MIFARE_ULTRALIGHT__SERIAL_NUMBER_SIZE_IN_BYTES)
//
bool smartcard::device_t::dump_sn(
	_Out_ byte_t *p_buffer,
	_In_ size_t buffer_size
) const
{
	if (buffer_size < MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES)
		return false;

	dump_sn_internal(p_buffer);
	return true;
}
void smartcard::device_t::dump_sn(
	_Out_ std::vector<byte_t> &buffer
) const
{
	buffer.resize(MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES);
	dump_sn_internal(buffer.data());
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void smartcard::device_t::dump_internal(
	_Out_ byte_t *p_buffer
) const
{
	for (size_t i = 0; i < (MIFARE_ULTRALIGHT__SIZE_IN_BYTES / MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE); p_buffer += MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE, ++i)
	{
		auto BinaryBlock = m_apdu.Read_BinaryBlock(i, MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE);
		memory::copy__f(p_buffer, BinaryBlock.data, BinaryBlock.size);
	}
}

void smartcard::device_t::dump_sn_internal(
	_Out_ byte_t *p_buffer
) const
{
	auto BinaryBlock = m_apdu.Read_BinaryBlock(0, 8);
	memory::copy__f(p_buffer, BinaryBlock.data, 3);
	memory::copy__f(p_buffer + 3, BinaryBlock.data + 3, 4);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//void smartcard::device_t::save_to_file(
//	_In_ const wchar_t *file_name,
//	_In_ const byte_t buffer[MIFARE_ULTRALIGHT__SIZE_IN_BYTES]
//) const
//{
//	std::ofstream f_out(file_name, std::wofstream::binary);
//	f_out.write(reinterpret_cast<const char*>(buffer), MIFARE_ULTRALIGHT__SIZE_IN_BYTES);
//	f_out.close();
//}
//
//void smartcard::device_t::save_to_file(
//	_In_ const wchar_t *file_name,
//	_In_ const byte_t *p_buffer, _In_ size_t buffer_size
//) const
//{
//	if (MIFARE_ULTRALIGHT__SIZE_IN_BYTES != buffer_size)
//		throw apdu::error::out_of_range_t(__FUNCTIONW__, buffer_size, MIFARE_ULTRALIGHT__SIZE_IN_BYTES);
//	
//	save_to_file(file_name, p_buffer);
//}

void smartcard::device_t::save_to_file(
	_In_ const wchar_t *file_name,
	_In_ const std::vector<byte_t> &buffer
) const
{
	std::ofstream f_out(file_name, std::wofstream::binary);
	f_out.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	f_out.close();
}

array_t<byte_t> smartcard::device_t::get_version() const
{
	return m_apdu.GetVersion();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------