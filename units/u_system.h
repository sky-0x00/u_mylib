/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// u_system.h - модуль описание основных (системных) типов данных и макроопределений
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

typedef   signed char      int08;		// не компилится, если явно определить зноковость
typedef   signed short     int16;
typedef   signed int       int32;
typedef   signed __int64   int64;

typedef float		real_s;
typedef double		real_d;
typedef long double real_e;

typedef std::wstring string_t;

typedef wchar_t  char_t;
typedef wchar_t* chars_t;
typedef const wchar_t* const_chars_t;		// ибо typeid(wchar_t *const) == typeid(const chars_t)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#ifdef S_OK
//#undef S_OK
//#endif

//#define S_OK			0		// все хорошо (так и должно быть)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define sign(x)   ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
//#define abs(x)    ((x) < 0 ? -(x) : (x))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define SORT_TYPE_ASC   0       // сортировка по возрастанию
//#define SORT_TYPE_DESC  1		// сортировка по убыванию

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
	// минимум из двух чисел
	template <typename type> type min(const type &a, const type & b) {
		return (a < b) ? a : b;
	}
	// максимум из двух чисел
	template <typename type> type max(const type &a, const type &b) {
		return (a > b) ? a : b;
	}

	// проверка числа x на вхождение в диапазон [a, b]
	template <typename type> bool in_range(const type &x, const type &a, const type &b) {
		return (x >= a) && (x <= b);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// обмен значениями двух переменных произвольного типа - нет альтернативы для языка C
	template <typename type> void exchange(_in _out type &a, _in _out type &b) {
		type buf = a;
		a = b;
		b = buf;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// сравнение двух объектов, вывод -1, если (a < b), 1, если (a > b), 0 если (a = b)
	template <typename type> char compare(_in const type &a, _in const type &b) {
		if (a < b)
			return -1;
		if (a > b)
			return 1;
		return 0;
	}

	// возвращает знак числа: -1 для отриц., 1 для положит. и 0 для нуля
	template <typename type> char sign(_in const type &x) {
		return compare<type>(x, static_cast<const type&>(0))
	}

	// подсчет числа элементов до первого элемента равного x (вместо предиката равенства можно использовать более общий предикат-шаблон)
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
// сортировка элементов массива
/*template <typename type> void sort_asc(in_out type *data, in uint32 count) {   // по возрастанию
	if (count > 1)
		for (uint32 i = 0; i < count - 1; i++)
			for (uint32 j = i; j < count; j++)
				if (data[i] > data[j])
					exchange<type>(data[i], data[j]);
}
template <typename type> void sort_desc(in_out type *data, in uint32 count) {   // по убыванию
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
