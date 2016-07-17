#include "stdafx.h"

#include "u_md5.h"
#include "..\..\u_convert.h"

// внутренн€€ функци€ - пишет в буфер, не провер€€ соответствие размеру буфера
//static void to_string(
//	_in const byte_t *data,
//	_out char_t *buffer,
//	_in string::case_type case_type
//) {
//	
//}

bool crypto::crc::md5::hash::to_string(
	_out char_t *buffer,
	_in size_t buffer_size,
	_in string::case_type case_type //= string::case_type::upper
) const {
	if (is_null(buffer))
	{
		// trace...
		return false;
	}
	if (buffer_size <= (HASH_SIZE__MD5 << 1))
	{
		// trace...
		return false;
	}
	
	convert::unsafe::bytes__to__string_hex(bytes, HASH_SIZE__MD5, buffer, case_type);
	buffer[HASH_SIZE__MD5 << 1] = L'\0';

	return true;
}

string_t crypto::crc::md5::hash::to_string(
	_in string::case_type case_type //= string::case_type::upper
) const {
	string_t result;
	result.reserve(HASH_SIZE__MD5 << 1);
	for (auto byte : bytes)
	{
		char_t buffer[2];
		convert::unsafe::byte__to__string_hex(byte, buffer, case_type);
		result.push_back(buffer[0]);
		result.push_back(buffer[1]);
	}
	return result;
}

bool crypto::crc::md5::hash::from_string(
	_in const string_t &hash
) {
	const constchar_array_t array = { hash.c_str(), hash.size() };
	return from_string(array);
}
bool crypto::crc::md5::hash::from_string(
	_in const char_t *hash
) {
	if (is_null(hash))
	{
		// trace...
		return false;
	}
	const constchar_array_t array = { hash, string::get_length(hash) };
	return from_string(array);
}
bool crypto::crc::md5::hash::from_string(
	_in const constchar_array_t &hash
) {
	if (hash.size != (HASH_SIZE__MD5 << 1))
	{
		// trace <размер не совпадает с ожидаемаым>
		return false;
	}

	// пропущенна€ логика
	return true;
}

crypto::crc::md5::hash& crypto::crc::md5::hash::operator=(
	const crypto::crc::md5::hash &hash
) {
	a = hash.a;
	b = hash.b;
	c = hash.c;
	d = hash.d;

	return *this;
}