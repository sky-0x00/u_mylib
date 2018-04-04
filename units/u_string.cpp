#include "stdafx.h"

#include "u_string.h"
#include "u_stdex.h"
//
///*inline*/ const char_t* string::empty()
//{
//	static const char_t result[] = { string::character::null() };
//	return result;
//}
///*inline*/ bool string::is_null(const char_t *str)
//{
//	return nullptr == str;
//}
///*inline*/ bool string::is_empty(const char_t *str)
//{
//	return *str == string::character::null();
//}
///*inline*/ bool string::check(const char_t *str)
//{
//	return !is_null(str) && !is_empty(str);
//}
//
//constexpr /*inline*/ char_t string::character::null()
//{
//	return L'\0';
//}
//string::character::digit_type string::character::is_digit(char_t ch)
//{
//	if (in_range(ch, L'0', L'9'))
//		return digit_type::dec;
//
//	if (in_range(ch, L'A', L'F') || in_range(ch, L'a', L'f'))
//		return digit_type::hex;
//
//	return digit_type::none;
//}
//
//size_t string::get_length(const char_t *str)
//{
//	size_t result = 0;
//	while (string::character::null() != str[result])
//		++result;
//	return result;
//}
//
//void string::reverse(const char_array_t &char_array)
//{
//	for (size_t half_length = char_array.size >> 1; half_length--; )
//		exchange(char_array.data[half_length], char_array.data[char_array.size - (half_length + 1)]);
//}
//void string::reverse(char_t *str)
//{
//	char_array_t str_native = { str, get_length(str) };
//	string::reverse(str_native);
//}

string::string()
{}
string::string(
	_in cstr_t str
) :
	//std::wstring( stdex::pointer::is_null(str) ? L"" : str)						// м€гкое поведение (объект строки создаетс€ дл€ nullptr-данных)
	std::wstring( str )																// жесткое поведение (бросаетс€ исключение дл€ nullptr-данных)
{}
string::string(
	_in cstr_t array,
	_in size_t count
) :
	std::wstring(array, count)
{}
string::string(
	_in char_t ch, 
	_in size_t repeat /*= 1*/
) :
	std::wstring(repeat, ch)
{}
//string::string(
//	_in const string &str
//) :
//	std::wstring(str)
//{}
string::string(
	_in const std::wstring &str
):
	std::wstring(str)
{}

/*static*/ bool string::check(
	_in cstr_t str
) noexcept
{
	return !stdex::pointer::is_null(str) && !stdex::string::is_empty(str);
}

void string::reverse()
{
	reverse(begin(), rbegin().base());
}
void string::reverse(
	_in iterator it_from,
	_in iterator it_to
) {
	if (it_to <= it_from)
		return;

	for (; it_from < --it_to; ++it_from)
		stdex::exchange<value_type>(*it_from, *it_to);
}

string& string::operator=(
	_in const std::wstring &str
) {
	assign(str);
	return *this;
}