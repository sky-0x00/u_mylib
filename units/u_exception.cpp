#include "stdafx.h"
#include "u_exception.h"
#include "u_string.h"
#include "u_convert.h"
#include <memory>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
exception::base::base() noexcept {}

/*explicit*/ exception::base::base(const char_t* message) noexcept
	: m_message(message) {}

exception::base::base(const exception::base& other) noexcept
	: m_message(other.m_message) {}

/*virtual*/ exception::base::~base() noexcept {}

exception::base& exception::base::operator=(const exception::base& other) noexcept
{
	if (this != &other)
		m_message = other.m_message;

	return *this;
}

void exception::base::set_message(
	arg_in cstr_t message
) noexcept
{
	m_message = string(L"исключение: [") + message + L"]";
}
/*virtual*/ void exception::base::set_message() noexcept {}

const char_t* exception::base::get_message() const noexcept
{
	if (m_message.empty())
	{
		typedef void (base::*pfn__set_message)(const char_t *);
		typedef void (base::*pfn__set_message__const)(const char_t *) const;
		pfn__set_message pfn = &base::set_message;
		pfn__set_message__const pfn_c = reinterpret_cast<pfn__set_message__const>(pfn);
		(this->*pfn_c)(L"неизвестное исключение");
	}		

	return m_message.c_str();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<> void exception::out_of_range<byte_t>::set_message() noexcept
{
	constexpr static auto infinity = std::numeric_limits<byte_t>::max();
	
	// ждем VS2015 Update 2
	//constexpr auto size = convert::compute_max_string_length<byte_t>();
	//char_t buffer[size + 1];

	// а пока так
	char_t buffer[4];
	convert::unsafe::uint32__to__string_dec(value, buffer);

	string exception = string(L"значение ") + buffer + L" находится вне допустимого диапазона значений [";

	convert::unsafe::uint32__to__string_dec(range[0], buffer);
	exception += buffer + string(L";");

	if (range[1] < infinity)
	{
		convert::unsafe::uint32__to__string_dec(range[1], buffer);
		exception += buffer + string(L"]");
	} else
		exception += L"+inf)";

	base::set_message(exception.c_str());
}

template<> void exception::out_of_range<size_t>::set_message() noexcept
{
	constexpr static auto infinity = std::numeric_limits<size_t>::max();

	// ждем VS2015 Update 2
	//constexpr auto size = convert::compute_max_string_length<size_t>();
	//char_t buffer[size + 1];

	// а пока так
	char_t buffer[4];
	convert::unsafe::uint32__to__string_dec(value, buffer);

	string exception = string(L"значение ") + buffer + L" находится вне допустимого диапазона значений [";

	convert::unsafe::uint32__to__string_dec(range[0], buffer);
	exception += buffer + string(L";");

	if (range[1] < infinity)
	{
		convert::unsafe::uint32__to__string_dec(range[1], buffer);
		exception += buffer + string(L"]");
	}
	else
		exception += L"+inf)";

	base::set_message(exception.c_str());
}

static string get_error_string(dword_t e_code, module_t h_module, bool is_trace_exception = true) noexcept
{
	const DWORD flag = h_module ? FORMAT_MESSAGE_FROM_HMODULE : FORMAT_MESSAGE_FROM_SYSTEM;
	void *buffer = nullptr;

	auto buffer_free = [](void *buffer)
	{
		return WinApi::Native::LocalFree(*reinterpret_cast<void**>(buffer));
	};
	std::unique_ptr<void*, decltype(buffer_free)> sp_buffer(&buffer, buffer_free);

	auto chars_count = WinApi::Native::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | flag,
		h_module,
		e_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(sp_buffer.get()),
		0, nullptr);

	if (chars_count)
	{
		LPCTSTR result = static_cast<LPCTSTR>(*sp_buffer);
		while (chars_count--)
		{
			const char_t &ch = result[chars_count];
			if ((ch != L'\n') && (ch != L'\r'))
				break;
		}
		return string(result, chars_count + 1);
	}
	
	if (is_trace_exception)
		exception::windows();

	return string::special::empty();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ string exception::windows::to_string(dword_t e_code, module_t h_module) noexcept
{
	return get_error_string(e_code, h_module, true);
}

exception::windows::windows(dword_t e_code, module_t h_module) noexcept 
	: e_code(e_code), h_module(h_module) {}

/*virtual*/ void exception::windows::set_message() noexcept
{
	char_t buffer[(sizeof(e_code) << 1) + 1];

	convert::unsafe::bytes__to__string_hex(e_code, buffer);
	string exception = string(L"код ошибки: 0x") + buffer + L" находится вне допустимого диапазона значений [";

	//convert::unsafe::uint32__to__string_dec(range[0], buffer);
	//exception += buffer + string_t(L";");

	//convert::unsafe::uint32__to__string_dec(range[1], buffer);
	//exception += buffer + string_t(L"]");

	base::set_message(exception.c_str());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------