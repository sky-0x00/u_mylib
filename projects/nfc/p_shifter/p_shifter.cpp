// p_shifter.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <locale.h>
#include <stdexcept>
#include <string>

typedef unsigned char byte_t;
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

void bytes_exchange(byte_t &lhs, byte_t &rhs)
{
	byte_t buf = lhs;
	lhs = rhs;
	rhs = buf;
}

template <typename type_t>
struct array_t
{
	type_t *data;
	size_t size;
	
	array_t(type_t *data, size_t size) : data(data), size(size) {}
};

template <typename type_t>
void bytes_reverse(type_t &value)
{
	array_t<byte_t> array(reinterpret_cast<byte_t*>(&value), sizeof(type_t));
	for (auto hsize = array.size >> 1; hsize; --hsize)
		bytes_exchange(array.data[hsize - 1], array.data[array.size - hsize]);
}

bool do_step(uint64_t value, uint64_t expected_value)
{
	wprintf(L"\n");

	// возвращает число типа uint64_t, у которого n младших бит выставлены в 0
	auto get_masked_value__lsb_nulls = [](size_t n = 0) -> uint64_t
	{
		return 0xFFFFFFFFFFFFFFFF << n;
	};
	// возвращает число типа uint64_t, у которого n старших бит выставлены в 0
	auto get_masked_value__msb_nulls = [get_masked_value__lsb_nulls](size_t n = 0) -> uint64_t
	{
		return get_masked_value__lsb_nulls(n) >> n;
	};

	// заменяем k последних бит числа value на 0
	for (size_t k = 0; k < 64; ++k)
	{
		auto value_k = value & get_masked_value__lsb_nulls(k);

		// заменяем i первых бит числа value на 0
		for (size_t i = 0; i < 64; ++i)
		{
			auto value_i = value_k & get_masked_value__msb_nulls(i);
			wprintf(L"k: %2u, i: %2u, value: 0x%016I64X\n", k, i, value_i);

			// сдвиг вправо число value на j бит
			for (size_t j = 0; j < 64; ++j)
			{
				if (value_i < expected_value)
					break;

				if (value_i == expected_value)
				{
					wprintf(L"j: %u, value_j: 0x%016I64X\n", j, value_i);
					return true;
				}

				value_i >>= 1;
			}
		}
	}

	return false;
}

void do_step_and_inform(uint64_t value, uint64_t expected_value)
{
	if (do_step(value, expected_value))
	{
		wprintf(L"значение найдено");
		exit(0);
	}
}

int main()
{
	{
		const wchar_t *pszLocaleName = L"Russian_Russia.866";
		if (!_wsetlocale(LC_ALL, pszLocaleName))
			wprintf(L"error: _wsetlocale(%s) failed", pszLocaleName);
	}

	uint64_t dump = 0x45DA2935CF4F9D00;			// 64 бит
	const uint64_t expected_value = 2472342777;

	do_step_and_inform(dump, expected_value);
	
	bytes_reverse(dump);
	do_step_and_inform(dump, expected_value);

	wprintf(L"увы, значение не найдено");
    return -1;
}

