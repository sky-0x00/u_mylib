#include "stdafx.h"
#include "u_string.h"

/*inline*/ const char_t* string::empty()
{
	static string_t result(L"");
	return result.c_str();
}

inline char_t string::character::null()
{
	return L'\0';
}

string::character::digit_type string::character::is_digit(char_t ch)
{
	if (in_range(ch, L'0', L'9'))
		return digit_type::dec;

	if (in_range(ch, L'A', L'F') || in_range(ch, L'a', L'f'))
		return digit_type::hex;

	return digit_type::none;
}

size_t string::get_length(const char_t *str)
{
	size_t result = 0;
	while (string::character::null() != str[result])
		++result;
	return result;
}

void string::reverse(const char_array_t &char_array)
{
	for (size_t half_length = char_array.size >> 1; half_length--; )
		exchange(char_array.data[half_length], char_array.data[char_array.size - (half_length + 1)]);
}
void string::reverse(char_t *str)
{
	const char_array_t str_native = { str, get_length(str) };
	string::reverse(str_native);
}
