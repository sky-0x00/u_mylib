#pragma once
#include <string>

typedef std::wstring string_t;

#include "u_scard_wrapper.h"
#include "u_mifare_ultralight.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace smartcard
{
	typedef CSmartcardContext context_t;
	//typedef ULONG status_t;

	class device_t
	{
	public:
		device_t(_In_ const context_t &context);

		string_list_t enum_all() const;
		void connect(_In_ const wchar_t *name);

		void read_data(_In_ byte_t page, _In_ byte_t bytes_count, _Out_ apdu::content_t &content) const;

	private:
		CSmartcardManager m_SmartcardManager;
	};
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------