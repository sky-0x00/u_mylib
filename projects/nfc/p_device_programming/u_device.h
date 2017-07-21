#pragma once
#include <string>

#include "u_scard_wrapper.h"
#include "u_mifare_ultralight.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace smartcard
{
	class device_t
	{
	public:
		device_t();

		string_list_t enum_all() const;
		void connect(_In_ const wchar_t *name);

		array_t<byte_t> read_data(_In_ size_t page, _In_ size_t bytes_count) const;

		size_t determine_pagecount();
		
		//void dump(_Out_ byte_t buffer[MIFARE_ULTRALIGHT__SIZE_IN_BYTES]) const;
		//bool dump(_Out_ byte_t *p_buffer, _In_ size_t buffer_size) const;
		size_t dump(_Out_ std::vector<byte_t> &buffer) const _NOEXCEPT;

		//void dump(_In_ const wchar_t *file_name) const;

		void dump_sn(_Out_ byte_t buffer[MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES]) const;
		bool dump_sn(_Out_ byte_t *p_buffer, _In_ size_t buffer_size) const;
		void dump_sn(_Out_ std::vector<byte_t> &buffer) const;

		//void save_to_file(_In_ const wchar_t *file_name, _In_ const byte_t buffer[MIFARE_ULTRALIGHT__SERIAL_NUMBER__SIZE_IN_BYTES]) const;
		//void save_to_file(_In_ const wchar_t *file_name, _In_ const byte_t *p_buffer, _In_ size_t buffer_size) const;
		void save_to_file(_In_ const wchar_t *file_name, _In_ const std::vector<byte_t> &buffer) const;

		array_t<byte_t> get_version() const;

	private:
		void dump_internal(_Out_ byte_t *p_buffer) const;
		void dump_sn_internal(_Out_ byte_t *p_buffer) const;

		CSmartcardContext m_context;
		CSmartcardManager m_manager;
		APDU_Manager m_apdu;

		size_t m_pagecount;
	};
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------