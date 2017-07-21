// p_device_programming.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "u_device.h"

#include <locale.h>
#include <string>
#include <stdexcept>
//#include <datetimeapi.h>
//#include <chrono>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <array>
#include <iomanip>

#include <locale>
#include <codecvt>

#include <fcntl.h>
#include <io.h>

#define format_hex( bytes ) std::hex << std::setw( bytes << 1 ) << std::setfill( L'0' ) << std::uppercase

//enum class case_type { cs_lower, ct_upper };

namespace std
{
	template < typename type >
	static bool in_range( IN const type &value, IN const std::pair< type, type > &range )
	{
		return ( value >= range.first ) && ( value <= range.second );
	}
}

//static wchar_t hbyte_to_hex(
//	_In_ size_t value,
//	_In_ case_type case_type
//) {
//	if (value < 10)
//		return static_cast<wchar_t>(value + '0');
//	if (value < 16)
//	{
//		value -= 10;
//		return static_cast<wchar_t>((case_type == case_type::ct_upper) ? value + 'A' : value + 'a');
//	}
//	throw std::out_of_range("value: " + std::to_string(value) + ", upper_bound: 16");
//}

//// без завершения символом '\0'
//void byte_to_hex(
//	_In_ size_t value, 
//	_Out_ wchar_t str[2], 
//	_In_ case_type case_type = case_type::ct_upper
//) {
//	str[0] = hbyte_to_hex(value >> 4, case_type);			// high-part
//	str[1] = hbyte_to_hex(value & 0xF, case_type);			// low-part
//}
//
//// без завершения символом '\0'
//void bytes_to_hex(
//	_In_ const byte_t *data,
//	_In_ size_t count,
//	_Out_ wchar_t *str,
//	_In_ case_type case_type = case_type::ct_upper
//) {
//	for (size_t i = 0; i < count; ++i)
//		byte_to_hex(data[i], str + (i << 1), case_type);
//}

static void set_locale_ru()
{
	const wchar_t *pszLocaleName = L"Russian_Russia.866";
	if (!_wsetlocale(LC_ALL, pszLocaleName))
		wprintf(L"error: _wsetlocale(%s) failed", pszLocaleName);

	const auto res = _setmode( _fileno(stdout), _O_U8TEXT );
	assert( -1 != res );
}

namespace echo
{
	static void hello()
	{
		//wprintf( UTILITY_NAME L", v%u.%u.%u от " TEXT(__TIMESTAMP__) L"\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD );
		wprintf( L"Утилита: " UTILITY_NAME L"\nВерсия: %u.%u.%u от %c%c%c%c-%c%c-%c%c " /*TEXT(__TIME__)*/ L"\nРабочая директория: \"" SAVING_DIR L"\"\n\n", 
			VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD,
			DATE_YEAR_CH0, DATE_YEAR_CH1, DATE_YEAR_CH2, DATE_YEAR_CH3,  DATE_MONTH_CH0, DATE_MONTH_CH1,  DATE_DAY_CH0, DATE_DAY_CH1 );
	}

	static void dump_raw( IN const std::vector<byte_t> &dump_raw )
	{
		wprintf( L"Дамп (raw-данные): %u байт\n  ", dump_raw.size() );

		unsigned n = 0;
		for each ( auto byte in dump_raw )
		{
			wprintf( L"%02X", byte );
			if ( !(++n % 4) )
				wprintf( L" " );
			if ( !(n % (8*4)) )
				wprintf( L"\n  " );
		}

		wprintf( L"\n\n" );
	}

	static void get_human_info( 
		IN const mifare_ultralight::format::human &dump_human, 
		OUT std::wostringstream &s_out 
	) {
		auto get_date_string = []( IN const std::tm &date )		-> string_t
		{
			std::array< wchar_t, 11> str;
			swprintf_s( str.data(), str.size(), L"%02u.%02u.%04u", date.tm_mday, 1 + date.tm_mon, 1900 + date.tm_year );
			return str.data();
		};
		auto get_time_string = []( IN const std::tm &time )		-> string_t
		{
			std::array< wchar_t, 9> str;
			swprintf_s( str.data(), str.size(), L"%02u:%02u:%02u", time.tm_hour, time.tm_min, time.tm_sec );
			return str.data();
		};

		auto get_days_str = []( IN uint16_t days )			-> const wchar_t*
		{
			static const std::array< const wchar_t *, 3 > days_strs = 
			{
				L"день", L"дня", L"дней"
			};
				
			switch ( days % 10 )
			{
				case 1:
					if ((days / 10) % 10 != 1)
						return days_strs[0];
					break;

				case 2:
				case 3:
				case 4:
					if ((days / 10) % 10 != 1)
						return days_strs[1];
					break;
			}
			return days_strs[2];
		};

		s_out << L"Информация на билете:" << std::endl << L"  uid: ";
		for ( unsigned i = 0; i < MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES; ++i )
			s_out << format_hex(1) << dump_human.uid[i];
		s_out << std::endl << L"  Серийный номер (на обороте): " << std::dec << dump_human.serial;
		s_out << std::endl << L"  Срок действия: " << dump_human.validity << L" " << get_days_str( dump_human.validity ) << std::endl 
			<< L"    с " << get_date_string( dump_human.tm_dateofsale ) << L" по " << get_date_string( dump_human.tm_validto ) << L" (включительно)";
		s_out << std::endl << L"  Кол-во поездок (оставшихся): " << dump_human.counter;
		s_out << std::endl << L"  Последнее использование: " << get_date_string( dump_human.tm_lastuse ) << L" " << get_time_string( dump_human.tm_lastuse ) 
			<< std::endl << L"    турникет: " << format_hex(2) << dump_human.wicket_no;				// TODO: выводить станцию метро
		s_out << std::endl << L"  Хэш (magic): " << format_hex(4) << dump_human.hash;
		s_out << std::endl << std::endl;
	}

	static void dump_human( 
		IN const std::wostringstream &s_out 
	) {
		wprintf( s_out.str().c_str() );
	}
}

#ifdef DUMP_FILENAME
namespace dump
{
	static void load_raw( 
		IN const wchar_t *file_name, 
		OUT std::vector<byte_t> &dump_raw 
	) {
		assert( file_name );
		std::ifstream f( file_name, std::ios_base::binary );
		assert( f.is_open() );
		f.seekg( 0, std::ios_base::end );
		dump_raw.resize( static_cast< size_t >( f.tellg() ) );
		f.seekg( 0, std::ios_base::beg );
		f.read( reinterpret_cast< char* >( dump_raw.data() ), dump_raw.size() );		
		assert( f.gcount() == dump_raw.size() );
	}

	static void save_human( 
		IN const string_t &filename,
		IN std::wostringstream &s_out 
	) {
		std::wofstream f_out( filename, std::ios_base::trunc );		// пишем как есть - иначе кириллица все ломает и на выход ничего не попадает
		assert( f_out.is_open() );
		
		// будем выводить в utf-8 с добавлением bom-маркера
		const auto locale_utf8 = std::locale( std::locale(), new std::codecvt_utf8_utf16< wchar_t, 0x10ffff, std::codecvt_mode::generate_header >() );
		f_out.imbue( locale_utf8 );

		f_out << s_out.str();
	}
}
#endif

int main()
{
	//typedef std::chrono::duration<unsigned int, std::ratio<24 * 60 * 60, 1>> days_t;
	//
	//std::tm day_begin { 0 };		// 1992-01-01
	//day_begin.tm_year = 92;
	//day_begin.tm_mday = 0;			// на день ранее
	//
	//std::tm day_end { 0 };			// 2016-03-07
	//day_end.tm_year = 116;
	//day_end.tm_mon = 2;
	//day_end.tm_mday = 7;

	//day_begin.tm_isdst = day_end.tm_isdst = 0;

	//const std::chrono::system_clock::time_point times[] = { std::chrono::system_clock::from_time_t(mktime(&day_begin)), std::chrono::system_clock::from_time_t(mktime(&day_end)) };
	//auto time_diff = std::chrono::duration_cast<days_t>(times[1] - times[0]);
	//
	//unsigned int days_from_start = time_diff.count();
	try
	{
		set_locale_ru();
		echo::hello();

		std::vector<byte_t> dump_raw;			// raw-данные дампа карты
		string_t filepath = SAVING_DIR;		

#ifndef DUMP_FILENAME
 		smartcard::device_t device;

		wprintf( L"> подключение к устройству чтения/записи смарт-карт..." );
		auto device_names = device.enum_all();
		if (device_names.empty())
		{
			wprintf( L" ошибка: не найдено доступных устройств\n" );
			return -1;
		}

		auto device_name = device_names.front().c_str();
		device.connect(device_name);
		wprintf( L" ok\n> успешное подключение к устройству \"%s\"\n", device_name );

		/*wprintf(L"определение число страниц памяти карты...", device_name);
		auto pagecount = device.determine_pagecount();
		wprintf(L" ok, %u страниц(-ы)\n", pagecount);

		switch (get_cardtype(pagecount))
		{
			case cardtype_e::ultralight:
				wprintf(L"тип карты: mifare ultralight (16 страниц по 4 байта)\n");
				break;
			case cardtype_e::ultralight_ev1_20p:
				wprintf(L"тип карты: mifare ultralight ev1 (20 страниц по 4 байта + one way counter)\n");
				break;
			case cardtype_e::ultralight_ev1_41p:
				wprintf(L"тип карты: mifare ultralight ev1 (41 страница по 4 байта + one way counter)\n");
				break;

			default:
				wprintf(L"ошибка: неподдерживаемый тип карты (не mifare ultralight?)\n");
				return -1;
		}*/


		//apdu::content_t content;
		//content.reserve(MIFARE_ULTRALIGHT__SIZE_IN_BYTES);
		//auto version_raw = device.get_version();

		wprintf( L"> считывание смарт-карты..." );
		auto pagecount = device.dump( dump_raw );
		wprintf( L" ok\n");

		byte_t smcard_uid[ MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES ];
		mifare_ultralight::format::human::get_uid( reinterpret_cast< const mifare_ultralight::format::raw * >( dump_raw.data() )->uid_begin, smcard_uid );

		std::array< wchar_t, (MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES << 1) + 1 > smcard_uidstr;
		for ( unsigned i = 0; i < _countof(smcard_uid); ++i )
			swprintf_s( smcard_uidstr.data() + (i << 1), 2, L"%08X", smcard_uid[i] );

		wprintf( L"> карта успешно считана - uid: %s, страниц: %u\n", smcard_uidstr, pagecount );
		
		//device.dump(L"c:\\mifare_u.dmp");

		filepath += string_t( L"UID_" ) + smcard_uidstr.data() + string_t( L".dump" );
		wprintf( L"> запись raw-дампа в файл \"%s\"...", filepath.c_str() );
		device.save_to_file( filepath.c_str(), dump_raw );
		wprintf( L" ok\n" );

#else
		filepath += DUMP_FILENAME;
		wprintf( L"> загрузка raw-дампа из файла \"%s\"...", filepath.c_str() );
		dump::load_raw( SAVING_DIR DUMP_FILENAME, dump_raw );
		wprintf( L" ok\n\n" );
#endif
		echo::dump_raw( dump_raw );

		wprintf( L"> разбор структуры raw-дампа...\n\n" );
		mifare_ultralight::format::human dump_human;
		dump_human.get( dump_raw );

		std::wostringstream s_out;
		echo::get_human_info( dump_human, s_out );
		echo::dump_human( s_out );

		// удаляем расширение у имени файла, если такое есть
		const auto pos_found = filepath.rfind( L'.' );
		if ( pos_found != string_t::npos )
			filepath.erase( pos_found );
		filepath += L".info";

		wprintf( L"> запись human-дампа в файл \"%s\"...", filepath.c_str() );
		dump::save_human( filepath.c_str(), s_out );
		wprintf( L" ok\n" );

	}
	catch (...)
	{
		return -1;
	}

    return 0;
}

