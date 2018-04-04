#pragma once

#include "u_system.h"
#include "u_macros.h"
#include <utility>

namespace stdex
{
#ifdef __cplusplus

	// минимум из двух чисел
	template <typename type_t> type_t min(
		_in const type_t &a,
		_in const type_t &b
	) {
		return (a < b) ? a : b;
	}
	// максимум из двух чисел
	template <typename type_t> type_t max(
		_in const type_t &a,
		_in const type_t &b
	) {
		return (a > b) ? a : b;
	}

	// проверка числа value на вхождение в диапазон [range_a, range_b]
	template <typename type_t> bool in_range(
		_in const type_t &value,
		_in const type_t &range_a,
		_in const type_t &range_b
	) {
		return (value >= range_a) && (value <= range_b);
	}
	// проверка числа value на вхождение в диапазон [range.first, range.second]
	template <typename type_t> bool in_range(
		_in const type_t &value,
		_in const std::pair<type_t, type_t> &range
	) {
		return in_range<type_t>( value, range.first, range.second );
	}

#else
	#define max(a, b)							((a) > (b) ? (a) : (b))
	#define min(a, b)							((a) < (b) ? (a) : (b))
	#define in_range(value, range_a, range_b)	( ((value) >= (range_a)) && ((value) <= (range_b)) )			// два макроса с одинаковым именем, хм...
	#define in_range(value, range)				in_range(value, range.first, range.second)
#endif

	// обмен значениями двух переменных произвольного типа - нет альтернативы для языка C
	template <typename type_t> void exchange(
		_in _out type_t &a,
		_in _out type_t &b
	) {
		type_t buf = a;
		a = b;
		b = buf;
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------
	// сравнение двух объектов, вывод -1: если (a < b), 1: если (a > b), 0: если (a == b)
	template <typename type_t> char compare(
		_in const type_t &a,
		_in const type_t &b
	) {
		if (a < b)
			return -1;
		if (a > b)
			return 1;
		return 0;
	}

	// возвращает знак числа: -1 для отриц., 1 для положит. и 0 для нуля
	template <typename type_t> char sign(
		_in const type_t &value
	) {
		return compare<type_t>(value, 0);
	}
	// модуль числа
	template <typename type_t> type_t abs(
		_in const type_t &value
	) {
		return (value < 0) ? -value : value;
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------
	// проверка указателя на нулевое значение
	namespace pointer
	{
		inline bool is_null(
			_in cpvoid_t ptr
		) noexcept 
		{
			return nullptr == ptr;
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------
	namespace string
	{
		namespace character
		{
			namespace special
			{
				constexpr inline char_t null()		{ return L'\0';	}
				constexpr inline char_t newline()	{ return L'\n'; }
			}
		}

		inline bool is_empty(
			_in cstr_t str
		) {
			return character::special::null() == *str;			// исключение, если nullptr == str
		}
	}
	
	//----------------------------------------------------------------------------------------------------------------------------------------------------

}	// namespace stdex