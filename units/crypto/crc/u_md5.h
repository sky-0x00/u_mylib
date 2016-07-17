#pragma once

#include "..\..\u_system.h"
#include "..\..\u_string.h"

#define HASH_SIZE__MD5		16			// 128 бит данных

namespace crypto
{
	namespace crc
	{
		namespace md5
		{
			union hash
			{
				struct 
				{
					uint32 a, b, c, d;
				};
				byte_t bytes[HASH_SIZE__MD5];

				hash& operator=(const hash &hash);

				// true - в случае успеха конвертации, false - в случае неудачи (либо буфер мал, либо он null)
				bool to_string(_out char_t *buffer, _in size_t buffer_size, _in string::case_type case_type = string::case_type::upper) const;
				string_t to_string(_in string::case_type case_type = string::case_type::upper) const;

				bool from_string(_in const string_t &hash);
				bool from_string(_in const char_t *hash);
				bool from_string(_in const constchar_array_t &hash);

			};

			class digest
			{
			public:
				void init(const hash &hash);
				void get_hash(hash &hash);
			private:
				hash hash;
			};
		}
	}
}