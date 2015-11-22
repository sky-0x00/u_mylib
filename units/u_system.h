/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// u_system.h - ������ �������� �������� (���������) ����� ������ � ����������������
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//#include <u_macros.h>
#include <string>

#define _in
#define _out

//#ifdef _MSC_VER
//#define inline __inline
//#endif

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

typedef std::wstring string_t;

typedef wchar_t  char_t;
typedef wchar_t* chars_t;
typedef const wchar_t* const_chars_t;		// ��� typeid(wchar_t *const) == typeid(const chars_t)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#ifdef S_OK
//#undef S_OK
//#endif

//#define S_OK			0		// ��� ������ (��� � ������ ����)

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

#ifdef __cplusplus

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������� �� ���� �����
	template <typename type> type min(const type &a, const type & b) {
		return (a < b) ? a : b;
	}
	// �������� �� ���� �����
	template <typename type> type max(const type &a, const type &b) {
		return (a > b) ? a : b;
	}

	// �������� ����� x �� ��������� � �������� [a, b]
	template <typename type> bool in_range(const type &x, const type &a, const type &b) {
		return (x >= a) && (x <= b);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����� ���������� ���� ���������� ������������� ���� - ��� ������������ ��� ����� C
	template <typename type> void exchange(_in _out type &a, _in _out type &b) {
		type buf = a;
		a = b;
		b = buf;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��������� ���� ��������, ����� -1, ���� (a < b), 1, ���� (a > b), 0 ���� (a = b)
	template <typename type> char compare(_in const type &a, _in const type &b) {
		if (a < b)
			return -1;
		if (a > b)
			return 1;
		return 0;
	}

	// ���������� ���� �����: -1 ��� �����., 1 ��� �������. � 0 ��� ����
	template <typename type> char sign(_in const type &x) {
		return compare<type>(x, static_cast<const type&>(0))
	}

	// ������� ����� ��������� �� ������� �������� ������� x (������ ��������� ��������� ����� ������������ ����� ����� ��������-������)
	template <typename type> size_t count_until_not_equal(_in const type *x_array, _in const type &x) {
		size_t count = 0;
		while (x_array[count] != x)
			count++;
		return count;
	}

#else

	#define max(a, b) ((a) > (b) ? (a) : (b))
	#define min(a, b) ((a) < (b) ? (a) : (b))
	#define in_range(x, a, b) (((x) >= (a)) && ((x) <= (b)) ? 1 : 0)

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���������� ��������� �������
/*template <typename type> void sort_asc(in_out type *data, in uint32 count) {   // �� �����������
	if (count > 1)
		for (uint32 i = 0; i < count - 1; i++)
			for (uint32 j = i; j < count; j++)
				if (data[i] > data[j])
					exchange<type>(data[i], data[j]);
}
template <typename type> void sort_desc(in_out type *data, in uint32 count) {   // �� ��������
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
