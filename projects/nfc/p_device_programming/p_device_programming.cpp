// p_device_programming.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "u_device.h"

#include <locale.h>
#include <string>
#include <stdexcept>
//#include <datetimeapi.h>
#include <chrono>

#define SAVING_DIR		L"s:\\lang\\u_mylib\\projects\\nfc\\data\\"

enum class case_type { cs_lower, ct_upper };

static wchar_t hbyte_to_hex(
	_In_ size_t value,
	_In_ case_type case_type
) {
	if (value < 10)
		return static_cast<wchar_t>(value + '0');
	if (value < 16)
	{
		value -= 10;
		return static_cast<wchar_t>((case_type == case_type::ct_upper) ? value + 'A' : value + 'a');
	}
	throw std::out_of_range("value: " + std::to_string(value) + ", upper_bound: 16");
}

// без завершения символом '\0'
void byte_to_hex(
	_In_ size_t value, 
	_Out_ wchar_t str[2], 
	_In_ case_type case_type = case_type::ct_upper
) {
	str[0] = hbyte_to_hex(value >> 4, case_type);			// high-part
	str[1] = hbyte_to_hex(value & 0xF, case_type);			// low-part
}

// без завершения символом '\0'
void bytes_to_hex(
	_In_ const byte_t *data,
	_In_ size_t count,
	_Out_ wchar_t *str,
	_In_ case_type case_type = case_type::ct_upper
) {
	for (size_t i = 0; i < count; ++i)
		byte_to_hex(data[i], str + (i << 1), case_type);
}

int main()
{
	typedef std::chrono::duration<unsigned int, std::ratio<24 * 60 * 60, 1>> days_t;
	
	std::tm day_begin { 0 };		// 1992-01-01
	day_begin.tm_year = 92;
	day_begin.tm_mday = 0;			// на день ранее
	
	std::tm day_end { 0 };			// 2016-03-07
	day_end.tm_year = 116;
	day_end.tm_mon = 2;
	day_end.tm_mday = 7;

	day_begin.tm_isdst = day_end.tm_isdst = 0;

	const std::chrono::system_clock::time_point times[] = { std::chrono::system_clock::from_time_t(mktime(&day_begin)), std::chrono::system_clock::from_time_t(mktime(&day_end)) };
	auto time_diff = std::chrono::duration_cast<days_t>(times[1] - times[0]);
	
	unsigned int days_from_start = time_diff.count();

	try
	{
		{
			const wchar_t *pszLocaleName = L"Russian_Russia.866";
			if (!_wsetlocale(LC_ALL, pszLocaleName))
				wprintf(L"error: _wsetlocale(%s) failed", pszLocaleName);
		}

 		smartcard::device_t device;

		wprintf(L"подключение к устройству чтения/записи смарт-карт...\n");
		auto device_names = device.enum_all();
		if (device_names.empty())
		{
			wprintf(L"ошибка: не найдено доступных устройств\n");
			return -1;
		}

		auto device_name = device_names.front().c_str();
		device.connect(device_name);
		wprintf(L"успешное подключение к устройству \"%s\"\n", device_name);

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

		wprintf(L"считывание смарт-карты...\n");
		std::vector<byte_t> dump_raw;
		auto pagecount = device.dump(dump_raw);

		wchar_t sn_raw[(MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES << 1) + 1];
		const auto &header = *reinterpret_cast<const mifare::ultralight_t::header_t*>(dump_raw.data());
		bytes_to_hex(header.sn_part_1, sizeof(header.sn_part_1), sn_raw);
		bytes_to_hex(header.sn_part_2, sizeof(header.sn_part_2), sn_raw + (sizeof(header.sn_part_1) << 1));
		sn_raw[_countof(sn_raw) - 1] = L'\0';
		wprintf(L"смарт-карта SN(UID): 0x%s успешно считана, %u страниц(-ы)\n", sn_raw, pagecount);
		
		//device.dump(L"c:\\mifare_u.dmp");

		const std::wstring &file_name = SAVING_DIR + std::wstring(L"uid_") + std::wstring(sn_raw) + std::wstring(L".dmp");
		wprintf(L"запись дампа смарткарты в файл \"%s\"...\n", file_name.c_str());
		device.save_to_file(file_name.c_str(), dump_raw);
		wprintf(L"запись в файл успешна\n");

	}
	catch (...)
	{
		return -1;
	}

    return 0;
}

