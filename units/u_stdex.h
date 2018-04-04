#pragma once

#include "u_system.h"
#include "u_macros.h"
#include <utility>

namespace stdex
{
#ifdef __cplusplus

	// ������� �� ���� �����
	template <typename type_t> type_t min(
		_in const type_t &a,
		_in const type_t &b
	) {
		return (a < b) ? a : b;
	}
	// �������� �� ���� �����
	template <typename type_t> type_t max(
		_in const type_t &a,
		_in const type_t &b
	) {
		return (a > b) ? a : b;
	}

	// �������� ����� value �� ��������� � �������� [range_a, range_b]
	template <typename type_t> bool in_range(
		_in const type_t &value,
		_in const type_t &range_a,
		_in const type_t &range_b
	) {
		return (value >= range_a) && (value <= range_b);
	}
	// �������� ����� value �� ��������� � �������� [range.first, range.second]
	template <typename type_t> bool in_range(
		_in const type_t &value,
		_in const std::pair<type_t, type_t> &range
	) {
		return in_range<type_t>( value, range.first, range.second );
	}

#else
	#define max(a, b)							((a) > (b) ? (a) : (b))
	#define min(a, b)							((a) < (b) ? (a) : (b))
	#define in_range(value, range_a, range_b)	( ((value) >= (range_a)) && ((value) <= (range_b)) )			// ��� ������� � ���������� ������, ��...
	#define in_range(value, range)				in_range(value, range.first, range.second)
#endif

	// ����� ���������� ���� ���������� ������������� ���� - ��� ������������ ��� ����� C
	template <typename type_t> void exchange(
		_in _out type_t &a,
		_in _out type_t &b
	) {
		type_t buf = a;
		a = b;
		b = buf;
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------
	// ��������� ���� ��������, ����� -1: ���� (a < b), 1: ���� (a > b), 0: ���� (a == b)
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

	// ���������� ���� �����: -1 ��� �����., 1 ��� �������. � 0 ��� ����
	template <typename type_t> char sign(
		_in const type_t &value
	) {
		return compare<type_t>(value, 0);
	}
	// ������ �����
	template <typename type_t> type_t abs(
		_in const type_t &value
	) {
		return (value < 0) ? -value : value;
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------
	// �������� ��������� �� ������� ��������
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
			return character::special::null() == *str;			// ����������, ���� nullptr == str
		}
	}
	
	//----------------------------------------------------------------------------------------------------------------------------------------------------

}	// namespace stdex