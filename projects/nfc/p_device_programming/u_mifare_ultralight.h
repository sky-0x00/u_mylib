#pragma once

#include <vector>

#include "u_scard_wrapper.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef unsigned char byte_t;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MIFARE_ULTRALIGHT__PAGE_COUNT		16
#define MIFARE_ULTRALIGHT__BYTES_PER_PAGE	 4
#define MIFARE_ULTRALIGHT__SIZE_IN_BYTES	(MIFARE_ULTRALIGHT__PAGE_COUNT * MIFARE_ULTRALIGHT__BYTES_PER_PAGE)

#define MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES		7
#define MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE			   16

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// типы карт mifare ultralight (x pages by 4 bytes):
// ultralight - 16 pages
// ultralight_ev1 - 20 and 41 pages
enum class cardtype_e { unknown = 0, ultralight = 16, ultralight_ev1_20p = 20, ultralight_ev1_41p = 41 };

cardtype_e get_cardtype(size_t pagecount);

template <typename type_t>
struct array_t
{
	const type_t *data;
	size_t size;

	array_t(const type_t *data = nullptr, size_t size = 0) : data(data), size(size) {}
	
	//const type_t* begin() { return data; }
	//const type_t* end() { return data + size; }

	//const type_t* cbegin() const { return data; }
	//const type_t* cend() const { return data + size; }
};

namespace apdu
{
	typedef std::vector<byte_t> buffer_t;
	//typedef unsigned int status_t;

	struct packet_t
	{
		buffer_t send, recieve;
		static bool is_sw_ok(const byte_t sw[2]);
	};

	namespace error
	{
		class error_t
		{
		public:
			error_t(_In_ const wchar_t *function);

		protected:
			const wchar_t *m_function;
			void output_message(_In_ const wchar_t *message) const;
		};

		class sw_t 
			: public error_t
		{
		public:
			sw_t(
				_In_ const wchar_t *function, 
				_In_ const byte_t sw[2]
			);
		};

		class out_of_range_t 
			: public error_t
		{
		public:
			out_of_range_t(
				_In_ const wchar_t *function, 
				_In_ size_t value, 
				_In_ const size_t bounds[2]
			);
			out_of_range_t(
				_In_ const wchar_t *function,
				_In_ size_t value,
				_In_ size_t expected_value
			);
		};

		class bytes_count_not_equal_buffer_size_t 
			: public error_t
		{
		public:
			bytes_count_not_equal_buffer_size_t(
				_In_ const wchar_t *function,
				_In_ size_t buffer_size,
				_In_ size_t bytes_count
			);
		};
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class APDU_Manager
{
public:
	APDU_Manager(_In_ const CSmartcardManager &SmartcardManager);

	array_t<byte_t> Read_BinaryBlock(_In_ size_t Page, _In_ size_t BytesToRead) const;
	array_t<byte_t> GetVersion() const;

private:
	size_t GetBytesToRead(_In_ size_t Page, _In_ size_t BytesToRead) const;

	const CSmartcardManager &m_SmartcardManager;
	mutable apdu::packet_t m_packet;
};

namespace mifare
{
	struct ultralight_t
	{
		struct header_t
		{
			byte_t sn_part_1[3];
			byte_t bcc_part_1;
			byte_t sn_part_2[4];
			byte_t bcc_part_2;
			byte_t internal;
			byte_t lock[2];
			union
			{
				byte_t as_byte[4];
				unsigned __int32 as_uint32;
			} otp;
		} header;

		byte_t data[48];
	};
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------