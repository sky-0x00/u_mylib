#include "stdafx.h"
#include "u_device.h"

#include "u_scard_wrapper.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define THROW_SCARD_ERROR(error)		throw CSmartcardError(error, __FUNCTIONW__)

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace smartcard
{
	enum class protocol 
	{
		t0 = SCARD_PROTOCOL_T0,
		t1 = SCARD_PROTOCOL_T1
	};
	enum class sharemode
	{
		shared = SCARD_SHARE_SHARED,
		exclusive = SCARD_SHARE_EXCLUSIVE,
		direct = SCARD_SHARE_DIRECT
	};
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
smartcard::device_t::device_t(_In_ const smartcard::context_t &context)
	: m_SmartcardManager(context)
{
}

string_list_t smartcard::device_t::enum_all() const
{
	return m_SmartcardManager.EnumDevices();
}

void smartcard::device_t::connect(_In_ const wchar_t *name)
{
	auto error = m_SmartcardManager.Connect(
		name, 
		static_cast<ULONG>(smartcard::sharemode::shared), 
		static_cast<ULONG>(smartcard::protocol::t0) | static_cast<ULONG>(smartcard::protocol::t1)
	);

	if (SCARD_S_SUCCESS != error)
		THROW_SCARD_ERROR(error);
}

void smartcard::device_t::read_data(_In_ byte_t page, _In_ byte_t bytes_count, _Out_ apdu::content_t &content) const
{
	content.clear();

	APDU_ReadData APDU(m_SmartcardManager);
	auto error = APDU.Execute(page, bytes_count);

	if (SCARD_S_SUCCESS != error)
		THROW_SCARD_ERROR(error);

	auto p_result = APDU.GetData();
	if (p_result)
		content.assign(p_result->cbegin(), p_result->cend() - 2);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------