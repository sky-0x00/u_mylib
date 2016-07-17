// p_hash.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <array>

//typedef unsigned __int32 error_t;
//typedef const wchar_t *pcwstr_t;
//typedef const wchar_t pcwstr_t[];
typedef std::wstring string_t;
typedef unsigned char byte_t;
typedef std::vector<byte_t> bytes_t;
typedef unsigned __int32 dword_t;

namespace dword
{
	string_t to_string(_In_ dword_t data)
	{
		std::wostringstream result;
		result << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0') << data;
		return result.str();
	}

	dword_t revert(_In_ dword_t data)
	{
		return ((data << 24) & 0xFF000000)			// байт №3 (самый старший)
			| ((data << 8) & 0xFF0000)				// байт №2
			| ((data >> 8) & 0xFF00)				// байт №1
			| ((data >> 24) & 0xFF);				// байт №0 (самый младший)
	}

	dword_t swap_words(_In_ dword_t data)
	{
		return ((data << 16) & 0xFFFF0000)
			| ((data >> 16) & 0xFFFF);
	}
}

void dump_load(_In_ const wchar_t* file_name, _Out_ bytes_t &buffer, _In_ size_t bytes_to_load)
{
	buffer.clear();
	
	std::ifstream f(file_name);
	if (!f.is_open())
	{
		std::wostringstream error;
		error << L"не удается открыть файл \"" << file_name << L"\" для чтения, ошибка 0x" << dword::to_string(GetLastError());
		throw error.str().c_str();
	}

	buffer.resize(bytes_to_load);
	f.read(reinterpret_cast<char*>(buffer.data()), bytes_to_load);

	if (f.gcount() != bytes_to_load)
	{
		std::wostringstream error;
		error << L"данные считаны не полностью, " << f.gcount() << L"байт вместо " << bytes_to_load << L" необходимых";
		throw error.str().c_str();
	}

	f.close();
}

void prepare_locale()
{
	const wchar_t locale[] = L"Russian_Russia.866";
	if (!_wsetlocale(LC_ALL, locale))
	{
		std::wostringstream error;
		error << L"не удалось установить локаль \"" << locale << L"\"";
		throw error.str().c_str();
	}
}

dword_t get_hash(_In_ const bytes_t &dump)
{
	const size_t hash_offset = 40;
	return *reinterpret_cast<const dword_t*>(dump.data() + hash_offset);
}

//template <size_t size> 
//void image_encode_first(
//	_Out_ bytes_t &image,
//	_In_ const bytes_t &dump,
//	_In_ const byte_t (&always_include)[size]
//) {
//	image.resize(size);
//	
//	size_t image_index = 0;
//	for (auto dump_index : always_include)
//		image[image_index++] = dump[dump_index];
//}

template <size_t size>
class bits_t
{
public:

	bits_t() : m_msb(0), m_count(0)
	{
		m_data.fill(false);
	}

	bool inc()
	{
		size_t i = 0;

		for (; i < size; ++i)
		{
			if (!m_data[i])
			{
				m_data[i] = true;
				++m_count;
				break;
			}
			else {
				m_data[i] = false;
				--m_count;
			}
		}

		if ((i + 1) > m_msb)
			m_msb = i + 1;

		return i == size;
	}
	inline size_t count() const
	{
		return m_count;
	}
	inline size_t msb() const
	{
		return m_msb;
	}

	string_t to_string() const
	{
		string_t result;
		result.reserve(size);

		for (auto it = m_data.crbegin(); it != m_data.crend(); ++it)
			result.push_back(*it ? L'1' : L'0');

		return result;
	}

	byte_t operator[](size_t index) const
	{
		return m_data[index];			// без проверки границ диапазона [0; m_count)
	}

	//const bool* cbegin() const
	//{
	//	return m_data.data();
	//}
	//const bool* cend() const
	//{
	//	return m_data.data() + m_count;
	//}

private:
	std::array<bool, size> m_data;
	size_t m_msb;
	size_t m_count;
};

template <typename type_t>
struct range_t
{
	const type_t *cbegin, *cend;
};

template <size_t always_include_size, size_t always_exclude_size, size_t size>
void image_encode(
	_Out_ bytes_t &image,
	_In_ const bytes_t &dump,
	_In_ const byte_t(&always_include)[always_include_size],
	_In_ const byte_t(&always_exclude)[always_exclude_size],
	_In_ const bits_t<size> &bits
) {
	const size_t image_size = always_include_size + bits.count();
	static const size_t dump_size = dump.size();

	image.clear();

	static const range_t<byte_t> always_include_range = { std::cbegin(always_include), std::cend(always_include) },
		always_exclude_range = { std::cbegin(always_exclude), std::cend(always_exclude) };

	size_t bits_index = 0;
	for (size_t i = 0; i < dump_size; ++i)
	{
		if (image_size == image.size())
			break;

		// если позиция элемента найдена в массиве always_include, то добавим такой элемент
		if (always_include_range.cend != std::find(always_include_range.cbegin, always_include_range.cend, i))
		{
			image.push_back(dump[i]);
			continue;
		}

		// если позиция элемента найдена в массиве always_exclude, то пропустим такой элемент
		if (always_exclude_range.cend != std::find(always_exclude_range.cbegin, always_exclude_range.cend, i))
			continue;

		// обрабатываем оставшиеся элементы
		if (bits_index < bits.msb())
		{
			if (bits[bits_index])
				image.push_back(dump[i]);				
			++bits_index;
		}
	}
}

namespace hash
{
	dword_t calc(_In_ const bytes_t &image)
	{
		return 0;
	}

	template <size_t size>
	bool check(
		_In_ const dword_t (&hashes)[size],
		_In_ dword_t hash
	) {
		static const range_t<dword_t> hashes_range = { std::cbegin(hashes), std::cend(hashes) };
		return std::find(hashes_range.cbegin, hashes_range.cend, hash) != hashes_range.cend;
	}
}

class console
{
public:
	console()
	{
		m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (INVALID_HANDLE_VALUE == m_handle)
			throw GetLastError();
	}
	COORD get_pos()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(m_handle, &csbi))
			throw GetLastError();

		return csbi.dwCursorPosition;
	}
	void set_pos(const COORD &coord)
	{
		if (!SetConsoleCursorPosition(m_handle, coord))
			throw GetLastError();
	}

private:
	HANDLE m_handle;
};

int main()
{
	prepare_locale();

	constexpr size_t dump_size = 48;
	const wchar_t dump_file_name[] = LR"(s:\lang\u_mylib\projects\nfc\data\uid_1_34C45591D62576.dmp)";

	bytes_t dump,		// дамп карты
		image;			// образ, по которому строится контрольная сумма

	std::wcout << L"загрузка дампа карты из файла \"" << dump_file_name << L"\"...";
	dump_load(dump_file_name, dump, dump_size);
	std::wcout << L" ok, " << dump_size << L" байт считано" << std::endl;

	dword_t hashes[4] = { get_hash(dump) };		// прямой хэш
	hashes[1] = dword::revert(hashes[0]);		// хэш с перестановкой байт (обратный)
	hashes[2] = dword::swap_words(hashes[0]);	// хэш с перестановкой слов
	hashes[3] = dword::swap_words(hashes[1]);	// обратный хэш с перестановкой слов

	std::wcout << L"hashes: " << dword::to_string(hashes[0])
		<< L", " << dword::to_string(hashes[1])
		<< L", " << dword::to_string(hashes[2])
		<< L", " << dword::to_string(hashes[3])
		<< std::endl;

	// позиции байтов, которые всегда включаются в образ, по которому строится котрольная сумма
	const byte_t always_include[] = {
		0, 1, 2, 4, 5, 6, 7,						// uid
		// 12, 13, 14, 15,							// otp
		// 37										// число оставшихся поездок
	};
	// позиции байтов, которые всегда исключаются из образа, по которому строится котрольная сумма
	const byte_t always_exclude[] = {
		40, 41, 42, 43,								// hash
	};

	image.reserve(dump_size);

	// минимальное число байт в образе: _countof(always_include)
	// максимальное число байт в образе: dump.size() - _countof(always_exclude)
	constexpr struct
	{
		size_t min, max;
	} bounds = 
	{ 
		_countof(always_include), 
		dump_size - _countof(always_exclude) 
	};
	
	// число изменяющихся битов в значении value - в диапазоне [0; bounds.max - bounds.min)
	constexpr size_t bits_total = bounds.max - bounds.min;
	bits_t<bits_total> bits;
	
	std::wcout << L"bits - total: " << bits_total << L", msb: ";
	console console;
	const auto pos = console.get_pos();
	size_t bits_msb = 0;
	std::wcout << bits_msb;

	do {
		image_encode(image, dump, always_include, always_exclude, bits);
		const dword_t hash = hash::calc(image);
		
		if (hash::check<_countof(hashes)>(hashes, hash))
		{
			// хэш совпал, выводим bits и выходим
			std::wcout << std::endl << L"hash found: " << std::hex << std::setw(sizeof(hash) << 1) << std::setfill(L'0') << hash 
				<< std::endl << L"bits: " << bits.to_string() << std::endl;
			break;
		}
		
		// хэш пока не совпал
		if (bits_msb != bits.msb())
		{
			// надо обновить значение msb в окне консоли
			bits_msb = bits.msb();
			console.set_pos(pos);
			std::wcout << bits_msb;
		}
	} 
	while (!bits.inc());

	return 0;
}

