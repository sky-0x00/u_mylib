// p_device_programming.cpp: ���������� ����� ����� ��� ����������� ����������.
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

//// ��� ���������� �������� '\0'
//void byte_to_hex(
//	_In_ size_t value, 
//	_Out_ wchar_t str[2], 
//	_In_ case_type case_type = case_type::ct_upper
//) {
//	str[0] = hbyte_to_hex(value >> 4, case_type);			// high-part
//	str[1] = hbyte_to_hex(value & 0xF, case_type);			// low-part
//}
//
//// ��� ���������� �������� '\0'
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
		//wprintf( UTILITY_NAME L", v%u.%u.%u �� " TEXT(__TIMESTAMP__) L"\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD );
		wprintf( L"�������: " UTILITY_NAME L"\n������: %u.%u.%u �� %c%c%c%c-%c%c-%c%c " /*TEXT(__TIME__)*/ L"\n������� ����������: \"" SAVING_DIR L"\"\n\n", 
			VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD,
			DATE_YEAR_CH0, DATE_YEAR_CH1, DATE_YEAR_CH2, DATE_YEAR_CH3,  DATE_MONTH_CH0, DATE_MONTH_CH1,  DATE_DAY_CH0, DATE_DAY_CH1 );
	}

	static void dump_raw( IN const std::vector<byte_t> &dump_raw )
	{
		wprintf( L"���� (raw-������): %u ����\n  ", dump_raw.size() );

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
				L"����", L"���", L"����"
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

		s_out << L"���������� �� ������:" << std::endl << L"  uid: ";
		for ( unsigned i = 0; i < MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES; ++i )
			s_out << format_hex(1) << dump_human.uid[i];
		s_out << std::endl << L"  �������� ����� (�� �������): " << std::dec << dump_human.serial;
		s_out << std::endl << L"  ���� ��������: " << dump_human.validity << L" " << get_days_str( dump_human.validity ) << std::endl 
			<< L"    � " << get_date_string( dump_human.tm_dateofsale ) << L" �� " << get_date_string( dump_human.tm_validto ) << L" (������������)";
		s_out << std::endl << L"  ���-�� ������� (����������): " << dump_human.counter;
		s_out << std::endl << L"  ��������� �������������: " << get_date_string( dump_human.tm_lastuse ) << L" " << get_time_string( dump_human.tm_lastuse ) 
			<< std::endl << L"    ��������: " << format_hex(2) << dump_human.wicket_no;				// TODO: �������� ������� �����
		s_out << std::endl << L"  ��� (magic): " << format_hex(4) << dump_human.hash;
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
		std::wofstream f_out( filename, std::ios_base::trunc );		// ����� ��� ���� - ����� ��������� ��� ������ � �� ����� ������ �� ��������
		assert( f_out.is_open() );
		
		// ����� �������� � utf-8 � ����������� bom-�������
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
	//day_begin.tm_mday = 0;			// �� ���� �����
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

		std::vector<byte_t> dump_raw;			// raw-������ ����� �����
		string_t filepath = SAVING_DIR;		

#ifndef DUMP_FILENAME
 		smartcard::device_t device;

		wprintf( L"> ����������� � ���������� ������/������ �����-����..." );
		auto device_names = device.enum_all();
		if (device_names.empty())
		{
			wprintf( L" ������: �� ������� ��������� ���������\n" );
			return -1;
		}

		auto device_name = device_names.front().c_str();
		device.connect(device_name);
		wprintf( L" ok\n> �������� ����������� � ���������� \"%s\"\n", device_name );

		/*wprintf(L"����������� ����� ������� ������ �����...", device_name);
		auto pagecount = device.determine_pagecount();
		wprintf(L" ok, %u �������(-�)\n", pagecount);

		switch (get_cardtype(pagecount))
		{
			case cardtype_e::ultralight:
				wprintf(L"��� �����: mifare ultralight (16 ������� �� 4 �����)\n");
				break;
			case cardtype_e::ultralight_ev1_20p:
				wprintf(L"��� �����: mifare ultralight ev1 (20 ������� �� 4 ����� + one way counter)\n");
				break;
			case cardtype_e::ultralight_ev1_41p:
				wprintf(L"��� �����: mifare ultralight ev1 (41 �������� �� 4 ����� + one way counter)\n");
				break;

			default:
				wprintf(L"������: ���������������� ��� ����� (�� mifare ultralight?)\n");
				return -1;
		}*/


		//apdu::content_t content;
		//content.reserve(MIFARE_ULTRALIGHT__SIZE_IN_BYTES);
		//auto version_raw = device.get_version();

		wprintf( L"> ���������� �����-�����..." );
		auto pagecount = device.dump( dump_raw );
		wprintf( L" ok\n");

		byte_t smcard_uid[ MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES ];
		mifare_ultralight::format::human::get_uid( reinterpret_cast< const mifare_ultralight::format::raw * >( dump_raw.data() )->uid_begin, smcard_uid );

		std::array< wchar_t, (MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES << 1) + 1 > smcard_uidstr;
		for ( unsigned i = 0; i < _countof(smcard_uid); ++i )
			swprintf_s( smcard_uidstr.data() + (i << 1), 2, L"%08X", smcard_uid[i] );

		wprintf( L"> ����� ������� ������� - uid: %s, �������: %u\n", smcard_uidstr, pagecount );
		
		//device.dump(L"c:\\mifare_u.dmp");

		filepath += string_t( L"UID_" ) + smcard_uidstr.data() + string_t( L".dump" );
		wprintf( L"> ������ raw-����� � ���� \"%s\"...", filepath.c_str() );
		device.save_to_file( filepath.c_str(), dump_raw );
		wprintf( L" ok\n" );

#else
		filepath += DUMP_FILENAME;
		wprintf( L"> �������� raw-����� �� ����� \"%s\"...", filepath.c_str() );
		dump::load_raw( SAVING_DIR DUMP_FILENAME, dump_raw );
		wprintf( L" ok\n\n" );
#endif
		echo::dump_raw( dump_raw );

		wprintf( L"> ������ ��������� raw-�����...\n\n" );
		mifare_ultralight::format::human dump_human;
		dump_human.get( dump_raw );

		std::wostringstream s_out;
		echo::get_human_info( dump_human, s_out );
		echo::dump_human( s_out );

		// ������� ���������� � ����� �����, ���� ����� ����
		const auto pos_found = filepath.rfind( L'.' );
		if ( pos_found != string_t::npos )
			filepath.erase( pos_found );
		filepath += L".info";

		wprintf( L"> ������ human-����� � ���� \"%s\"...", filepath.c_str() );
		dump::save_human( filepath.c_str(), s_out );
		wprintf( L" ok\n" );

	}
	catch (...)
	{
		return -1;
	}

    return 0;
}

