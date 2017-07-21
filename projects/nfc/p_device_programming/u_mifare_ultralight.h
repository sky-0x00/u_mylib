#pragma once

#include <vector>

#include "u_scard_wrapper.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef unsigned char byte_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef const byte_t *cdata_t;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MIFARE_ULTRALIGHT__PAGE_COUNT		16
#define MIFARE_ULTRALIGHT__BYTES_PER_PAGE	 4
#define MIFARE_ULTRALIGHT__SIZE_IN_BYTES	(MIFARE_ULTRALIGHT__PAGE_COUNT * MIFARE_ULTRALIGHT__BYTES_PER_PAGE)

#define MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES		7
#define MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE			   16

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���� ���� mifare ultralight (x pages by 4 bytes):
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

namespace mifare_ultralight
{
	namespace format
	{
		struct raw
		{
		// page 0
			byte_t uid_begin[3];		// ������� ����� uid-������ �����
			byte_t bcc_0;				
		// page 1
			byte_t uid_end[4];			// ������� ����� uid-������ �����
		// page 2
			byte_t bcc_1;
			byte_t reserved_0;			// ? ������ [E0]
			uint16_t lock;				// ����� ����������, ������ [F0 00]
		// page 3
			uint32_t otp;				// ����������� otp-�����: one-time-programming (�� ���� �� �������?), ����������� 1 �� ���� ������������ �������
		// page 4, 5
			uint32_t serial_begin;		// ������� ����� ��������� ������ ������ (� ������� ����������� �����)
			uint32_t serial_end;		// ������� ����� ��������� ������ ������
		// page 6, 7
			uint16_t date_validto_0;	// ����, �� ������� ������� ����� (���� ������� + ����� ���� �������� - 1)
			uint16_t time_validto_0;	// �����, �� ������� ������� ����� (������-�� ����� ������ [00 00])
			uint16_t date_validto_1;	// ����, �� ������� ������� ����� - ������� ���� date_of_sale_0
			uint16_t time_validto_1;	// �����, �� ������� ������� ����� - ������� ���� time_of_sale_0
		// page 8
			uint16_t date_ofsale;		// ���� ������� ������
			uint16_t validity;			// ���� �������� � ���� (�� 1,2 ������� - 5 ����, ��������� - 90 ����)
		// page 9
			byte_t reserved_1;			// ? ������ [40]
			byte_t counter;				// ������� ���������� �������, ����������������, ����� ������� ����� � otp-������ (�� ����� ������� ������)
			uint16_t wicket_no;			// ����� ���������� ��������� ����� (�� ����� ���, �� ����� 0xFFFF?)
		// page 10
			uint32_t hash;				// crc �� ��������� ������������� ���� 16-3, �� �������� ������� � ��������� ����� (������ ������)
		// page 11
			uint16_t date_of_lastuse;	// ���� ��������� �������
			uint16_t time_of_lastuse;	// ����� ��������� �������
		// page 12-15 - ������� page 8-11

		// page 15-39 - ��������� 25 ������� ������ ��������� ��� (��� ������ �� 60 �������)
		  /*00000000 00000000 00000000 00000000
			00000000 00000000 00000000 00000000
			00000000 00000000 00000000 00000000
			00000000 00000000 00000000 00000000
			00000000 00000000 00000000 00000000
			000000bd 000000ff 00050000 00000000
			00000000*/

		};

		class human
		{
		public:
			byte_t uid[ MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES ];
			byte_t bcc[2];
			uint16_t lock;
			uint32_t otp;
			uint32_t serial;
			std::tm tm_validto;			// ����� ������ ������� (� ���� ����� ��� ������ �������)
			std::tm tm_dateofsale;		// ����� ������ ������� (� ���� ����� ��� ���� ������)
			uint16_t validity;
			byte_t counter;
			uint16_t wicket_no;			// TODO: ������� �� ��� ���������� (����� ����� �������� �������� �������)
			uint32_t hash;
			std::tm tm_lastuse;

			void get( IN const std::vector< byte_t > &dump_raw );

			static void get_uid( IN cdata_t uid_begin, OUT byte_t (&uid)[ MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES ] );

		private:
			void get_uid( IN cdata_t uid_begin );
			void get_serial( IN cdata_t serial_begin );
			bool check_bcc() const;

			static void decode_date( IN uint16_t encoded, OUT std::tm &decoded );
			static void decode_time( IN uint16_t encoded, OUT std::tm &decoded );
			static void init_and_decode_tm( OUT std::tm &decoded, IN const uint16_t *encoded_date, IN const uint16_t *encoded_time );
		};		
	}
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------