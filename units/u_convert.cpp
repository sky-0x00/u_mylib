#include "stdafx.h"
#include "u_convert.h"
#include "u_system.h"

#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif

template<> constexpr size_t convert::compute_max_string_length<uint08>() { return  3; }				// 0..255
template<> constexpr size_t convert::compute_max_string_length<uint16>() { return  5; }				// 0..65'535
template<> constexpr size_t convert::compute_max_string_length<uint32>() { return 10; }				// 0..4'294'967'295
template<> constexpr size_t convert::compute_max_string_length<uint64>() { return 20; }				// 0..18'446'744'073'709'551'614


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static 
char_t half_byte__to__hex_digit(
	byte_t value, 
	string::case_type case_type
) {
	if (value < 10)
		return value + '0';
	if (value < 16)
		return (case_type == string::case_type::upper) ? value - 10 + 'A' : value - 10 + 'a';
	
	throw exception::out_of_range<byte_t>(value, {0, 15});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void convert::unsafe::byte__to__string_hex(
	arg_in byte_t value, 
	arg_out char_t *buffer, 
	arg_in string::case_type case_type //= string::case_type::upper
) {
	buffer[0] = half_byte__to__hex_digit(value >> 4, case_type);
	buffer[1] = half_byte__to__hex_digit(value & 0x0F, case_type);
}

void byte__to__string_hex(
	arg_in byte_t value,
	arg_out char_t *buffer,
	arg_in size_t buffer_size,
	arg_in string::case_type case_type //= string::case_type::upper
) {
	if (buffer_size < 2)
		throw exception::out_of_range<size_t>(buffer_size, { 2, std::numeric_limits<size_t>::max() });

	convert::unsafe::byte__to__string_hex(value, buffer, case_type);
}

string convert::byte__to__string_hex(
	arg_in byte_t value, 
	arg_in string::case_type case_type //= string::case_type::upper
) {
	char_t buffer[2];
	convert::unsafe::byte__to__string_hex(value, buffer, case_type);
	return string(buffer, _countof(buffer));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void convert::unsafe::bytes__to__string_hex(
	arg_in const void *data,
	arg_in size_t size_in_bytes,
	arg_out char_t *buffer,
	arg_in string::case_type case_type //= string::case_type::upper
) {
	for (auto p_bytes = static_cast<const byte_t *>(data); size_in_bytes--; p_bytes++, buffer += 2)
		convert::unsafe::byte__to__string_hex(*p_bytes, buffer, case_type);
}

string convert::bytes__to__string_hex(
	arg_in const byte_t *values, 
	arg_in size_t count, 
	arg_in string::case_type case_type //= string::case_type::upper
) {
	string result;
	result.reserve(count << 1);
	wchar_t buffer[2];
	for (; count--; values++)
	{
		convert::unsafe::byte__to__string_hex(*values, buffer, case_type);
		result.append(buffer, _countof(buffer));
	}
	return result;
}
//template <size_t count> void convert::bytes__to__string_hex(
//	arg_in const byte_t values[count],
//	arg_out wchar_t *buffer,
//	arg_in string::case_type case_type //= string::case_type::upper
//) {
//	convert::bytes__to__string_hex(values, count, buffer, case_type);
//}
//void convert::bytes__to__string_hex(
//	arg_in const std::vector<byte_t> values,
//	arg_out char_t *buffer,
//	arg_in string::case_type case_type //= string::case_type::upper
//) {
//	bytes__to__string_hex(values.data(), values.size(), buffer, case_type);
//}
//string_t convert::bytes__to__string_hex(
//	arg_in const std::vector<byte_t> values,
//	arg_in string::case_type case_type //= string::case_type::upper
//) {
//	return bytes__to__string_hex(values.data(), values.size(), case_type);
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// вспомогательная функция: преобразует беззнаковое целое в строку, символы которой идут в обратном порядке
// возвращает число символов, записанных в результирующий буффер (без учета символа '\0')
//
template <typename uint_t>
static size_t uint__to__string_dec__reverse(
	arg_in uint_t value,
	arg_out char_t *buffer
) {
	static_assert(std::is_integral<uint_t>() && std::is_unsigned<uint_t>(), "функция легитимна только для ИНТЕГРАЛЬНЫХ БЕЗЗНАКОВЫХ типов данных");

	size_t count = 0;
	for (; value >= 10; value /= 10)
	{
		auto remainder = value % 10;
		buffer[count++] = static_cast<char_t>(remainder + '0');
	}
	buffer[count++] = static_cast<char_t>(value + '0');
	buffer[count] = L'\0';
	return count;
}

//
// преобразует БЕЗзнаковое целое в строку, представлюящую его десятичный эквивалент
//
template <typename uint_t>
static size_t uint__to__string_dec(
	arg_in uint_t value,
	arg_out char_t *buffer
) {
	static_assert(std::is_integral<uint_t>() && std::is_unsigned<uint_t>(), "функция легитимна только для ИНТЕГРАЛЬНЫХ БЕЗЗНАКОВЫХ типов данных");

	auto count = uint__to__string_dec__reverse(value, buffer);
	//const char_array_t char_array = { buffer, count };	
	//string::reverse(char_array);
	return count;
}

//
// преобразует ЗНАКОВОЕ целое в строку, представлюящую его десятичный эквивалент
//
template <typename int_t>
static size_t int__to__string_dec(
	arg_in int_t value,
	arg_out char_t *buffer
) {
	static_assert(std::is_integral<int_t>() && std::is_signed<int_t>(), "функция легитимна только для ИНТЕГРАЛЬНЫХ ЗНАКОВЫХ типов данных");
	typedef std::make_unsigned<int_t>::type uint_t;

	size_t count = 0;
	if (value < 0)
	{
		value = -value;
		buffer[count++] = L'-';
	}
	auto count_delta = uint__to__string_dec__reverse(static_cast<uint_t>(value), buffer + count);
	//const char_array_t char_array = { buffer + count, count_delta };
	//string::reverse(char_array);
	return count + count_delta;
}

size_t convert::unsafe::uint08__to__string_dec(arg_in uint08 value, arg_out char_t *buffer)
{
	return uint__to__string_dec(value, buffer);
}
size_t convert::unsafe::uint16__to__string_dec(arg_in uint16 value, arg_out char_t *buffer)
{
	return uint__to__string_dec(value, buffer);
}
size_t convert::unsafe::uint32__to__string_dec(arg_in uint32 value, arg_out char_t *buffer)
{
	return uint__to__string_dec(value, buffer);
}
size_t convert::unsafe::uint64__to__string_dec(arg_in uint64 value, arg_out char_t *buffer)
{
	return uint__to__string_dec(value, buffer);
}
size_t convert::unsafe::size__to__string_dec(arg_in size_t value, arg_out char_t *buffer)
{
	return uint__to__string_dec(value, buffer);
}


size_t convert::unsafe::int08__to__string_dec(arg_in int08 value, arg_out char_t *buffer)
{
	return int__to__string_dec(value, buffer);
}
size_t convert::unsafe::int16__to__string_dec(arg_in int16 value, arg_out char_t *buffer)
{
	return int__to__string_dec(value, buffer);
}
size_t convert::unsafe::int32__to__string_dec(arg_in int32 value, arg_out char_t *buffer)
{
	return int__to__string_dec(value, buffer);
}
size_t convert::unsafe::int64__to__string_dec(arg_in int64 value, arg_out char_t *buffer)
{
	return int__to__string_dec(value, buffer);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////