#include "stdafx.h"
#include "u_com.h"
#include "..\u_convert.h"

#ifdef WIN32
	#include "..\winapi\u_winapi.h"
#else
#endif

ports::com::com(): handle(nullptr)
{}

ports::com::status ports::com::open(number_t number, size_t mode) noexcept
{
	if (is_opened())
	{
		auto status = close_impl();
		if (status::ok != status)
			return status;
	}
	return open_impl(number, mode);
}

inline bool ports::com::is_opened() const noexcept
{
	return nullptr != handle;
}

ports::com::status ports::com::close() noexcept
{
	if (!is_opened())
		return status::ok;

	return close_impl();
}

ports::com::status ports::com::close_impl() noexcept
{
#ifdef WIN32
	if (WinApi::Native::CloseHandle(handle))
	{
		handle = nullptr;
		return status::ok;
	}

	return static_cast<ports::com::status>(WinApi::Native::GetLastError());
#else
#endif
}

ports::com::status ports::com::open_impl(number_t number, size_t mode) noexcept
{
#ifdef WIN32
	char_t fname[12] = LR"(\\.\COM)";
	convert::unsafe::uint08__to__string_dec(number, fname + 7);
	auto hFile = WinApi::Native::CreateFile(fname, 0, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		handle = hFile;
		return status::ok;
	}

	return static_cast<ports::com::status>(WinApi::Native::GetLastError());
#else
#endif
}