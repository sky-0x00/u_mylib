/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// u_system.h - ������ �������� �������� (���������) ����� ������ � ����������������
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//#include <u_macros.h>
#include <string>

#define arg_in
#define arg_out
#define arg_option

//#ifdef _MSC_VER
//#define inline __inline
//#endif

#define UNSUPPORTED_PLATFORM	���������������� ���������

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned char     uint08, byte_t;
typedef unsigned short    uint16, word_t;
typedef unsigned int      uint32, dword_t;
typedef unsigned __int64  uint64, qword_t;
typedef bool              bit_t;

typedef   signed char      int08;		// �� ����������, ���� ���� ���������� ����������
typedef   signed short     int16;
typedef   signed int       int32;
typedef   signed __int64   int64;

typedef float		real_s;
typedef double		real_d;
typedef long double real_e;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define sign(x)   ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
//#define abs(x)    ((x) < 0 ? -(x) : (x))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define SORT_TYPE_ASC   0       // ���������� �� �����������
//#define SORT_TYPE_DESC  1		// ���������� �� ��������

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define ROTATE_DWORD(x)   ((x << 8) | (x >> 8))				// ??

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus

	// ������� �� ���� �����
	template <typename type_t> type_t min(
		arg_in const type_t &a, 
		arg_in const type_t &b
	) {
		return (a < b) ? a : b;
	}
	// �������� �� ���� �����
	template <typename type_t> type_t max(
		arg_in const type_t &a, 
		arg_in const type_t &b
	) {
		return (a > b) ? a : b;
	}

	// �������� ����� value �� ��������� � �������� [from, to]
	template <typename type_t> bool in_range(
		arg_in const type_t &value, 
		arg_in const type_t &from, 
		arg_in const type_t &to
	) {
		return (value >= from) && (value <= to);
	}
	// �������� ����� value �� ��������� � �������� [range.first, range.second]
	template <typename type_t> bool in_range(
		arg_in const type_t &value,
		arg_in const std::pair<type_t, type_t> &range
	) {
		return in_range<type_t>(value, range.first, range.second);
	}

#else
	#define max(a, b) ((a) > (b) ? (a) : (b))
	#define min(a, b) ((a) < (b) ? (a) : (b))
	#define in_range(x, a, b) (((x) >= (a)) && ((x) <= (b)) ? 1 : 0)
	#define in_range(x, range) in_range(x, range.first, range.second)

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*template <typename type_t>
struct range_t
{
	type_t from, to;
	bool is_in(const type_t &value)
	{
		return in_range(value, from, to);
	}
};*/

// ������ ������ ������������� ���� � C-�����
template <typename type_t> struct array_t
{
	type_t *data;
	size_t size;

	//explicit array_t(const type_t *data, size_t size) : data(data), size(size) {}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����� ���������� ���� ���������� ������������� ���� - ��� ������������ ��� ����� C
template <typename type_t> void exchange(
	arg_in arg_out type_t &a, 
	arg_in arg_out type_t &b
) {
	type_t buf = a;
	a = b;
	b = buf;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������� ���� ��������, ����� -1, ���� (a < b), 1, ���� (a > b), 0 ���� (a = b)
template <typename type_t> char compare(
	arg_in const type_t &a, 
	arg_in const type_t &b
) {
	if (a < b)
		return -1;
	if (a > b)
		return 1;
	return 0;
}

// ���������� ���� �����: -1 ��� �����., 1 ��� �������. � 0 ��� ����
template <typename type_t> char sign(
	arg_in const type_t &x
) {
	return compare<type_t>(x, static_cast<const type_t&>(0))
}

// ������� ����� ��������� �� ������� �������� ������� x (������ ��������� ��������� ����� ������������ ����� ����� ��������-������)
template <typename type_t> size_t count_until_not_equal(
	arg_in const type_t *x_array, 
	arg_in const type_t &x
) {
	size_t count = 0;
	while (x_array[count] != x)
		count++;
	return count;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool is_null(
	arg_in const void *p
) {
	return nullptr == p;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���������� ��������� �������
/*template <typename type> void sort_asc(in_out type *data, arg_in uint32 count) {   // �� �����������
	if (count > 1)
		for (uint32 i = 0; i < count - 1; i++)
			for (uint32 j = i; j < count; j++)
				if (data[i] > data[j])
					exchange<type>(data[i], data[j]);
}
template <typename type> void sort_desc(in_out type *data, arg_in uint32 count) {   // �� ��������
	if (count > 1)
		for (uint32 i = 0; i < count - 1; i++)
			for (uint32 j = i; j < count; j++)
				if (data[i] < data[j])
					exchange<type>(data[i], data[j]);
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// placement syntax for operator new
/*inline void* operator new(uint32 size, void *p) {
	return p;
}
inline void* operator new[](uint32 size, void *p) {
	return p;
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
