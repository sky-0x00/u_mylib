/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// u_system.h - ����������� �������� (���������) ����� ������
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned char     uint08_t, byte_t;
typedef unsigned short    uint16_t, word_t;
typedef unsigned int      uint32_t, dword_t;
typedef unsigned __int64  uint64_t, qword_t;
typedef bool              bit_t;

typedef   signed char      int08_t;		// �� ����������, ���� ���� ���������� ����������
typedef   signed short     int16_t;
typedef   signed int       int32_t;
typedef   signed __int64   int64_t;

typedef float		real_s;
typedef double		real_d;
typedef long double real_e;

typedef void void_t, *pvoid_t;
typedef const void *cpvoid_t;

typedef wchar_t char_t, *str_t;
typedef const char_t *cstr_t;

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
//template <typename type_t> struct array_t
//{
//	type_t *data;
//	size_t size;
//
//	//explicit array_t(const type_t *data, size_t size) : data(data), size(size) {}
//};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������� ����� ��������� �� ������� �������� ������� x (������ ��������� ��������� ����� ������������ ����� ����� ��������-������)
//template <typename type_t> size_t count_until_not_equal(
//	arg_in const type_t *x_array, 
//	arg_in const type_t &x
//) {
//	size_t count = 0;
//	while (x_array[count] != x)
//		count++;
//	return count;
//}

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
