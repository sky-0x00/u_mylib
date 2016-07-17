#include "stdafx.h"

#include <cstdarg>
#include <ctime>
//#include <fstream>

#include "u_trace.h"
#include "winapi\u_winapi.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// настраиваемые параметры
//

// приращение размера буфера, если текуший размер недостаточен
#define BUFFER_INC_SIZE					32

//
#define BUFFER_CALC_LENGTH_ERROR		L"ошибка вычисления размера буфера трассировки"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ const std::map<trace::category, cstr_t> &tracer::buffer::category_prefixes =
{
	{ trace::category::normal, L"" },
	{ trace::category::error,  L"E> " }
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
char_t *p_data;

tracer::tracer(
	cstr_t module_name, 
	const config &config
) : 
	m_name(module_name), 
	m_format(config.format),
	m_buffer(module_name, config.initial_buffer_size)
{
	m_target_debug = config.targets.debug;
	if (string::check(config.targets.file))
	{
		m_fstream.open(config.targets.file, std::ios_base::out | std::ios_base::app);
		if (m_fstream.is_open())
		{
			;// out_to_buffer(m_buffer.data(), L"1");
		}
		else
			;//WinApi::Native::OutputDebugString(buffer_data);
	}
}

tracer::~tracer()
{
	if (m_fstream.is_open())
		m_fstream.close();
}

bool tracer::is_target__debug() const
{
	return m_target_debug;
}
bool tracer::is_target__file() const
{
	return m_fstream.is_open() ? true : false;
}
bool tracer::is_trace_needed() const
{
	return is_target__debug() || is_target__file();
}

void tracer::out_buffer_to__debug() const
{
	//static std::mutex mutex;
	//std::lock_guard
}

size_t tracer::trace(
	trace::category category,
	const char_t *format,
	...
	) {
	if (!string::check(format) || !is_trace_needed())
		return 0;

	va_list args;
	va_start(args, format);
	{
		std::lock_guard<std::mutex> lock(m_buffer.get_mutex());
		m_buffer.create_prefix(m_format);
		m_buffer.append_string(category, format, args);
	}
	va_end(args);
	//WinApi::Native::OutputDebugString(m_buffer.get_string());

	return m_buffer.get_length();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void tracer::buffer::output(
	const char_t *format, 
	va_list args
){
	// rest - остаточный буфер, он лежит в диапазоне [m_data.data(); m_data.data() + m_data.size())
	// число символов в остаточном буфере rest равно rest_size

	const size_t length = get_length();
	const size_t rest_size = m_data.size() - length;

	// выводим форматированную строку в буфер p_data
	auto count = vswprintf(p_data, rest_size, format, args);
	if (-1 == count)
	{
		// размер буфера в rest_size недостаточен, переустановим его
		count = _vscwprintf(format, args);
		if (-1 == count)
			throw BUFFER_CALC_LENGTH_ERROR;
	}
	if (count >= rest_size)
	{
		m_data.resize(++count + length);
		p_data = m_data.data() + length;
		count = vswprintf(p_data, count, format, args);
		if (-1 == count)
			throw BUFFER_CALC_LENGTH_ERROR;
	}
	p_data += count;
}

void tracer::buffer::output(
	const char_t *format, 
	...
) {
	va_list args;
	va_start(args, format);
	output(format, args);
	va_end(args);
}

tracer::buffer::buffer(
	arg_in const string &module_name,
	arg_in size_t initial_size
) :
	m_module_name(module_name),
	m_data(initial_size),								// 32 << 4 = 512 байт
	m_newdata_ptr(m_data.data())
{}

void tracer::buffer::create_prefix(
	const struct tracer::config::format &format
) {
	p_data = m_data.data();

	SYSTEMTIME Time;
	WinApi::Native::GetLocalTime(&Time);
	
	// выводим отметку времени
	if (format.date)
	{
		// "YYYY-MM-DD "
		output(L"%04u-%02u-%02u ", Time.wYear, Time.wMonth, Time.wDay);
	}
	output(L"%02u:%02u:%02u", Time.wHour, Time.wMinute, Time.wSecond);
	if (format.time_msecs)
	{
		// ".MSMS"
		output(L".%03u", Time.wMilliseconds);
	}
	
	// выводим имя модуля
	output(L" [%s]", m_module_name);

	// выводим информацию о сессии, процессе, потоке
	if (format.session_id)
	{
		const auto SessionId = WinApi::Session::Current::Id();
		if (SessionId == WinApi::Session::Id::Unknown)
			output(L" [?]");
		else
			switch (format.ids)
			{
			case config::format::ids::dec:
				output(L" [%u]", SessionId);
				break;
			case config::format::ids::hex:
				output(L" [0x%X]", SessionId);
				break;
			}
	}

	switch (format.ids)
	{
	case config::format::ids::dec:
		output(L" [%u:%u]", WinApi::Process::Current::Id(), WinApi::Thread::Current::Id());
		break;
	case config::format::ids::hex:
		output(L" [0x%X:0x%X]", WinApi::Process::Current::Id(), WinApi::Thread::Current::Id());
		break;
	}
}

void tracer::buffer::append_string(
	trace::category category, 
	const char_t *format, 
	va_list args
) {
	//out(L" ", category_prefixes.at(category), format, args, L"\n");
	
	static std::vector<char_t> format_str;
	auto &category_str = category_prefixes.at(category);
	
	//const auto size = 3 +string::length(category_str) + string::length(format);
	const auto size = 3;

	format_str.resize(size);
	swprintf_s(format_str.data(), size, L" %s%s\n", category_str, format);			// assert(size-1)	
	output(format_str.data(), args);
}

std::mutex& tracer::buffer::get_mutex() noexcept
{
	return m_mutex;
}

size_t tracer::buffer::get_length() const noexcept
{
	return p_data - m_data.data();
}
const char_t* tracer::buffer::get_data() const noexcept
{
	return m_data.data();
}

//#include <windows.h>
//#include <assert.h>
//#include <mutex>
//#include "winapi\u_winapi.h"
//
//tracer_t::tracer_t(const config_t &config):
//	info(config.info), buffer(config.get_buffer_size())
//{
//	initialize(config);
//}
//
//void tracer_t::initialize(const tracer_t::config_t &config)
//{
//	trace_to.debug_output = config.trace_to.debug_output;
//	
//	if (!config.trace_to.file_name)
//		return;
//
//	trace_to.file_stream.open(config.trace_to.file_name, std::ios_base::out | std::ios_base::app);
//}
//tracer_t::~tracer_t()
//{
//	if (trace_to.file_stream.is_open())
//		trace_to.file_stream.close();
//}
//
//size_t tracer_t::create_prefix(wchar_t *buffer_data, size_t buffer_size)
//{
//	auto prefix_time = [](wchar_t *buffer_data, size_t buffer_size, const trace::info_t &info)
//	{
//		register size_t result = 0;
//		SYSTEMTIME Time;
//		WinApi::Native::GetLocalTime(&Time);
//
//		if (info.date)
//		{
//			// "YYYY-MM-DD "
//			result += swprintf_s(buffer_data + result, buffer_size - result, L"%04u-%02u-%02uT", Time.wYear, Time.wMonth, Time.wDay);
//		}
//		// "HH:MM:SS"
//		result += swprintf_s(buffer_data + result, buffer_size - result, L"%02u:%02u:%02u", Time.wHour, Time.wMinute, Time.wSecond);
//		if (info.time_msecs)
//		{
//			// ".MSMS"
//			result += swprintf_s(buffer_data + result, buffer_size - result, L".%03u", Time.wMilliseconds);
//		}
//		return result;
//	};
//	auto prefix_id = [](wchar_t *buffer_data, size_t buffer_size, ULONG id, const trace::info_t &info)
//	{
//		register size_t result = 0;
//		wchar_t id_s[6];
//
//		assert(0 != id);
//
//		// пока при конвертации используем crt - через _itow_s()
//		switch (info.id_format)
//		{
//		case trace::info_t::id_format_t::hex:
//			assert(0 == _itow_s(id, id_s, 16));
//			result += swprintf_s(buffer_data + result, buffer_size - result, L"0x%s", id_s);
//			break;
//		case trace::info_t::id_format_t::dec:
//			assert(0 == _itow_s(id, id_s, 10));
//			result += swprintf_s(buffer_data + result, buffer_size - result, L"%s", id_s);
//			break;
//		case trace::info_t::id_format_t::hex_dec:
//			assert(0 == _itow_s(id, id_s, 16));
//			result += swprintf_s(buffer_data + result, buffer_size - result, L"0x%s", id_s);
//			assert(0 == _itow_s(id, id_s, 10));
//			result += swprintf_s(buffer_data + result, buffer_size - result, L"(%s)", id_s);
//			break;
//		}
//
//		return result;
//	};
//
//	register size_t result = prefix_time(buffer_data, buffer_size, info);
//	result += swprintf_s(buffer_data + result, buffer_size - result, L"%c[", trace::separator);
//	
//	result += prefix_id(buffer_data + result, buffer_size - result, WinApi::Process::Current::GetID(), info);
//	result += swprintf_s(buffer_data + result, buffer_size - result, L":");
//	result += prefix_id(buffer_data + result, buffer_size - result, WinApi::Thread::Current::GetID(), info);
//	result += swprintf_s(buffer_data + result, buffer_size - result, L"]%c", trace::separator);	
//
//	return result;
//}
//
//size_t tracer_t::trace(trace::category category, const wchar_t* format, ...)
//{
//	if (!is_trace_needed())
//		return 0;
//
//	const size_t buffer_size = buffer.size();
//	wchar_t *const buffer_data = buffer.data();
//
//	static std::mutex mutex;
//	std::lock_guard<std::mutex> locker(mutex);
//
//	register size_t result = create_prefix(buffer_data, buffer_size);
//
//	switch (category)
//	{
//	case trace::category::normal:
//		break;
//	case trace::category::error:
//		result += swprintf_s(buffer_data + result, buffer_size - result, L"ERROR> ");
//		break;
//	}
//
//	va_list args;
//	va_start(args, format);
//	result += _vswprintf_p(buffer_data + result, buffer_size - result, format, args);
//	va_end(args);
//
//	result += swprintf_s(buffer_data + result, buffer_size - result, L"\n");
//
//	if (trace_to.debug_output)
//		WinApi::Native::OutputDebugString(buffer_data);
//	if (trace_to.file_stream.is_open())
//		trace_to.file_stream << buffer_data;
//
//	return result;
//}
//
//inline bool tracer_t::is_trace_needed()
//{
//	return trace_to.debug_output || trace_to.file_stream.is_open();
//}
//
//tracer_t::config_t::config_t(size_t buffer_size) :
//	buffer_size(buffer_size), 
//	trace_to{ nullptr, false } {}
//
//size_t tracer_t::config_t::get_buffer_size() const
//{
//	return buffer_size;
//}
//
//trace::info_t::info_t() :
//	date(true), time_msecs(true), id_format(trace::info_t::id_format_t::hex) {}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
tracer::config::targets::targets() noexcept :
	debug(true), 
	file(nullptr) 
{}

tracer::config::format::format() noexcept :
	date(true),
	time_msecs(true),
	session_id(true),
	ids(ids::hex)
{}

tracer::config::config() noexcept :
	initial_buffer_size(BUFFER_INC_SIZE << 4)			// 32 << 4 = 512 байт
{}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------