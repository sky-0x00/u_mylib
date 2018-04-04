#pragma once

#include "u_system.h"
#include "u_macros.h"

#include <string>
#include <list>
#include <vector>

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

class string :
	public std::wstring
{
public:
	typedef std::list<string> list;
	typedef std::vector<string> vector;

	enum class case_type { lower, upper };

	string();
	string(_in cstr_t str);								// �� c-������ (c '\0' �� �����)
	string(_in cstr_t array, _in size_t count);		// �� ������� ������ count �������� (������ ����� ��������� '\0' � ������������ �������)
	string(_in char_t ch, _in size_t repeat = 1);		// �� �������, ������������ repeat ���
	//string(_in const string &str);						// ����������� �����������
	string(_in const std::wstring &str);					// �� std::wstring ������

	void reverse();
	void reverse(_in iterator it_from, _in iterator it_to);

	string& operator=(_in const std::wstring &str);

	static bool check( _in cstr_t str ) noexcept;

private:

};	// class string