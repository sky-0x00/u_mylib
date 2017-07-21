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

namespace mifare_ultralight
{
	namespace format
	{
		struct raw
		{
		// page 0
			byte_t uid_begin[3];		// старшая часть uid-номера карты
			byte_t bcc_0;				
		// page 1
			byte_t uid_end[4];			// младшая часть uid-номера карты
		// page 2
			byte_t bcc_1;
			byte_t reserved_0;			// ? всегда [E0]
			uint16_t lock;				// байты блокировок, всегда [F0 00]
		// page 3
			uint32_t otp;				// прожигаемые otp-байты: one-time-programming (на всех ли билетах?), заполняются 1 по мере расходования поездок
		// page 4, 5
			uint32_t serial_begin;		// старшая часть серийного номера билета (в системе московского метро)
			uint32_t serial_end;		// младшая часть серийного номера билета
		// page 6, 7
			uint16_t date_validto_0;	// дата, по которую валиден билет (дата продажи + число дней действия - 1)
			uint16_t time_validto_0;	// время, по которое валиден билет (почему-то здесь всегда [00 00])
			uint16_t date_validto_1;	// дата, по которую валиден билет - зеркало поля date_of_sale_0
			uint16_t time_validto_1;	// время, по которое валиден билет - зеркало поля time_of_sale_0
		// page 8
			uint16_t date_ofsale;		// дата продажи билета
			uint16_t validity;			// срок действия в днях (на 1,2 поездки - 5 дней, остальные - 90 дней)
		// page 9
			byte_t reserved_1;			// ? всегда [40]
			byte_t counter;				// счетчик оставшихся поездок, декрементируется, имеет жесткую связь с otp-полями (по числу поездок билета)
			uint16_t wicket_no;			// номер последнего турникета метро (их всего что, не более 0xFFFF?)
		// page 10
			uint32_t hash;				// crc по алгоритму иммитовставки ГОСТ 16-3, со сменными ключами и таблицами замен (полная засада)
		// page 11
			uint16_t date_of_lastuse;	// дата последней поездки
			uint16_t time_of_lastuse;	// время последней поездки
		// page 12-15 - зеркало page 8-11

		// page 15-39 - последние 25 страниц всегда заполнены так (для билета на 60 поездок)
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
			std::tm tm_validto;			// время всегда нулевое (в поле дампа эти данные нулевые)
			std::tm tm_dateofsale;		// время всегда нулевое (в поле дампа нет этих данных)
			uint16_t validity;
			byte_t counter;
			uint16_t wicket_no;			// TODO: создать БД для турникетов (можно будет выводить название станции)
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