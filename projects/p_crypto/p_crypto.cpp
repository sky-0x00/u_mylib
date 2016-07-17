// p_crypto.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "..\..\units\crypto\crc\u_md5.h"

int main()
{
	__asm 
	{
		mov eax, 0x11223344
	}

	crypto::crc::md5::hash hash;
	for (byte_t i = 0; i < HASH_SIZE__MD5; ++i)
		hash.bytes[i] = i;

	char_t buffer[(HASH_SIZE__MD5 << 1) + 1];
	string_t result;
	
	hash.to_string(buffer, _countof(buffer), string::case_type::upper);
	result = hash.to_string(string::case_type::upper);

	hash.to_string(buffer, _countof(buffer), string::case_type::lower);
	result = hash.to_string(string::case_type::lower);

    return 0;
}

