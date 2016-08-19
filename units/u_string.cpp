#include "stdafx.h"
#include "u_string.h"
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
	arg_in cstr_t str
) :
	std::wstring(is_null(str) ? string::special::empty() : str)
{}
string::string(
	arg_in cstr_t array,
	arg_in size_t count
) :
	std::wstring(array, count)
{}
string::string(
	arg_in char_t ch, 
	arg_in size_t repeat /*= 1*/
) :
	std::wstring(repeat, ch)
{}
//string::string(
//	arg_in const string &str
//) :
//	std::wstring(str)
//{}
string::string(
	arg_in const std::wstring &str
):
	std::wstring(str)
{}

/*static*/ bool string::check(
	arg_in cstr_t str
) noexcept 
{
	return !is_null(str) && !is_empty(str);
}
/*static*/ bool string::is_null(
	arg_in cstr_t str
) noexcept
{
	return ::is_null(str);
}
/*static*/ bool string::is_empty(
	arg_in cstr_t str
) {
	return *str == character::special::null();
}

void string::reverse()
{
	reverse(begin(), rbegin().base());
}
void string::reverse(
	arg_in iterator it_from,
	arg_in iterator it_to
) {
	if (it_to <= it_from)
		return;

	for (; it_from < --it_to; ++it_from)
		exchange<value_type>(*it_from, *it_to);
}

string& string::operator=(
	arg_in const std::wstring &str
) {
	assign(str);
	return *this;
}

/*static*/ constexpr char_t character::special::null()
{
	return L'\0';
}
/*static*/ constexpr char_t character::special::newline()
{
	return L'\n';
}

/*static*/ cstr_t string::special::empty()
{
	static const char_t char_null = character::special::null();
	return &char_null;
}

/*static*/ cstr_t string::special::newline()
{
	static const char_t chars_newline[] = { character::special::newline(), character::special::null() };
	return chars_newline;
}