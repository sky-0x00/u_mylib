#pragma once

#include "u_system.h"

#include <string>
#include <list>
#include <vector>

typedef wchar_t char_t;
typedef char_t *str_t;
typedef const char_t *cstr_t;

//typedef std::wstring string_t;
//typedef array_t<char_t> char_array_t;
//typedef array_t<const char_t> constchar_array_t;

//namespace string
//{
//	typedef string_t type_t;
//
//	namespace character
//	{
//		typedef char_t type_t;
//
//		constexpr /*inline*/ char_t null();
//		enum class digit_type { none, dec, hex };
//
//		digit_type is_digit(char_t ch);
//	}
//	
//	enum class case_type { lower, upper };
//
//	/*inline*/ const char_t* empty();
//	size_t get_length(const char_t *str);
//
//	/*inline*/ bool is_null(const char_t *str);
//	/*inline*/ bool is_empty(const char_t *str);
//	/*inline*/ bool check(const char_t *str);
//	
//	void reverse(char_t *str);
//	void reverse(const char_array_t &char_array);
//}

struct character
{
	struct special
	{
		constexpr static char_t null();			// '\0'
		constexpr static char_t newline();		// '\n'

	};	// struct special

};	// struct character

class string :
	public std::wstring
{
public:
	typedef std::list<string> list;
	typedef std::vector<string> vector;

	enum class case_type { lower, upper };

	struct special
	{
		static cstr_t empty();					// ""
		static cstr_t newline();				// "\n"

	};	// struct special

	string();
	string(arg_in cstr_t str);								// из c-строки (c '\0' на конце)
	string(arg_in cstr_t array, arg_in size_t count);		// из массива длиною count символов (массив может содержать '\0' в произвольной позиции)
	string(arg_in char_t ch, arg_in size_t repeat = 1);		// из символа, повторенного repeat раз
	//string(arg_in const string &str);						// конструктор копирования
	string(arg_in const std::wstring &str);					// из std::wstring строки

	void reverse();
	void reverse(arg_in iterator it_from, arg_in iterator it_to);

	string& operator=(arg_in const std::wstring &str);

	static bool check(arg_in cstr_t str) noexcept;
	static bool is_null(arg_in cstr_t str) noexcept;
	static bool is_empty(arg_in cstr_t str);

private:

};	// class string