#pragma once

#include "u_string.h"
#include "u_exception.h"

#include <vector>

namespace convert
{
	// рассчет макс. числа символов при конвертации дес€тичного беззнакового целого в строку
	// без учета символа терминатора '\0', мин. длина - 1 символ
	//namespace compute_max_string_length
	//{
	//	/*inline*/ constexpr size_t uint08__to__string_dec();
	//	/*inline*/ constexpr size_t uint16__to__string_dec();
	//	/*inline*/ constexpr size_t uint32__to__string_dec();
	//	/*inline*/ constexpr size_t uint64__to__string_dec();
	//}

	template <typename uint_t> constexpr size_t compute_max_string_length();

	namespace unsafe
	{
		//
		// преобразует Ѕ≈«знаковое целое в строку, представлю€щую его дес€тичный эквивалент
		// возвращает число символов, записанных в буфер
		//
		size_t uint08__to__string_dec(arg_in uint08 value, arg_out char_t *buffer);
		size_t uint16__to__string_dec(arg_in uint16 value, arg_out char_t *buffer);
		size_t uint32__to__string_dec(arg_in uint32 value, arg_out char_t *buffer);
		size_t uint64__to__string_dec(arg_in uint64 value, arg_out char_t *buffer);

		size_t size__to__string_dec(arg_in size_t value, arg_out char_t *buffer);

		//
		// преобразует «Ќј ќ¬ќ≈ целое в строку, представлю€щую его дес€тичный эквивалент
		// возвращает число символов, записанных в буфер
		//
		size_t int08__to__string_dec(arg_in int08 value, arg_out char_t *buffer);
		size_t int16__to__string_dec(arg_in int16 value, arg_out char_t *buffer);
		size_t int32__to__string_dec(arg_in int32 value, arg_out char_t *buffer);
		size_t int64__to__string_dec(arg_in int64 value, arg_out char_t *buffer);
	}


	// преобразование одиночного байта в строку (без финализации символом '\0')
	namespace unsafe
	{
		void byte__to__string_hex(
			arg_in byte_t value,
			arg_out char_t *buffer,
			arg_in string::case_type case_type = string::case_type::upper);
	}
	void byte__to__string_hex(
		arg_in byte_t value,
		arg_out char_t *buffer,
		arg_in size_t buffer_size,
		arg_in string::case_type case_type = string::case_type::upper);

	string byte__to__string_hex(
		arg_in byte_t value,
		arg_in string::case_type case_type = string::case_type::upper);

	namespace unsafe
	{
		// преобразование массива байт в C-строку без финализации символом '\0'
		void bytes__to__string_hex(
			arg_in const void *data,
			arg_in size_t size_in_bytes,
			arg_out char_t *buffer,
			arg_in string::case_type case_type = string::case_type::upper);

		// преобразование значени€ произвольного типа в C-строку без финализации символом '\0'
		template <typename type_t>
		void bytes__to__string_hex(
			const type_t &data,
			arg_out char_t *buffer,
			arg_in string::case_type case_type = string::case_type::upper
		) {
			bytes__to__string_hex(&data, sizeof(type_t), buffer, case_type);
		}
	}

	// преобразование массива байт в C++ строку
	string bytes__to__string_hex(
		arg_in const byte_t *values,
		arg_in size_t count,
		arg_in string::case_type case_type = string::case_type::upper);

	//template <size_t count> void bytes__to__string_hex(
	//	_in const byte_t values[count],
	//	_out char_t *buffer,
	//	_in string::case_type case_type = string::case_type::upper
	//) {
	//	convert::bytes__to__string_hex(values, count, buffer, case_type);
	//}
	//void bytes__to__string_hex(
	//	_in const std::vector<byte_t> values,
	//	_out char_t *buffer,
	//	_in string::case_type case_type = string::case_type::upper);
	//string_t bytes__to__string_hex(
	//	_in const std::vector<byte_t> values,
	//	_in string::case_type case_type = string::case_type::upper);

}	// namespace convert
