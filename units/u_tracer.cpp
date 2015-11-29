#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include <mutex>
#include "u_tracer.h"
#include "winapi\u_winapi.h"

tracer_t::tracer_t(const config_t &config):
	info(config.info), buffer(config.get_buffer_size())
{
	initialize(config);
}

void tracer_t::initialize(const tracer_t::config_t &config)
{
	trace_to.debug_output = config.trace_to.debug_output;
	
	if (!config.trace_to.file_name)
		return;

	trace_to.file_stream.open(config.trace_to.file_name, std::ios_base::out | std::ios_base::app);
}
tracer_t::~tracer_t()
{
	if (trace_to.file_stream.is_open())
		trace_to.file_stream.close();
}

size_t tracer_t::create_prefix(wchar_t *buffer_data, size_t buffer_size)
{
	auto prefix_time = [](wchar_t *buffer_data, size_t buffer_size, const trace::info_t &info)
	{
		register size_t result = 0;
		SYSTEMTIME Time;
		WinApi::Native::GetLocalTime(&Time);

		if (info.date)
		{
			// "YYYY-MM-DD "
			result += swprintf_s(buffer_data + result, buffer_size - result, L"%04u-%02u-%02uT", Time.wYear, Time.wMonth, Time.wDay);
		}
		// "HH:MM:SS"
		result += swprintf_s(buffer_data + result, buffer_size - result, L"%02u:%02u:%02u", Time.wHour, Time.wMinute, Time.wSecond);
		if (info.time_msecs)
		{
			// ".MSMS"
			result += swprintf_s(buffer_data + result, buffer_size - result, L".%03u", Time.wMilliseconds);
		}
		return result;
	};
	auto prefix_id = [](wchar_t *buffer_data, size_t buffer_size, ULONG id, const trace::info_t &info)
	{
		register size_t result = 0;
		wchar_t id_s[6];

		assert(0 != id);

		// пока при конвертации используем crt - через _itow_s()
		switch (info.id_format)
		{
		case trace::info_t::id_format_t::hex:
			assert(0 == _itow_s(id, id_s, 16));
			result += swprintf_s(buffer_data + result, buffer_size - result, L"0x%s", id_s);
			break;
		case trace::info_t::id_format_t::dec:
			assert(0 == _itow_s(id, id_s, 10));
			result += swprintf_s(buffer_data + result, buffer_size - result, L"%s", id_s);
			break;
		case trace::info_t::id_format_t::hex_dec:
			assert(0 == _itow_s(id, id_s, 16));
			result += swprintf_s(buffer_data + result, buffer_size - result, L"0x%s", id_s);
			assert(0 == _itow_s(id, id_s, 10));
			result += swprintf_s(buffer_data + result, buffer_size - result, L"(%s)", id_s);
			break;
		}

		return result;
	};

	register size_t result = prefix_time(buffer_data, buffer_size, info);
	result += swprintf_s(buffer_data + result, buffer_size - result, L"%c[", trace::separator);
	
	result += prefix_id(buffer_data + result, buffer_size - result, WinApi::Process::Current::GetID(), info);
	result += swprintf_s(buffer_data + result, buffer_size - result, L":");
	result += prefix_id(buffer_data + result, buffer_size - result, WinApi::Thread::Current::GetID(), info);
	result += swprintf_s(buffer_data + result, buffer_size - result, L"]%c", trace::separator);	

	return result;
}

size_t tracer_t::trace(trace::category category, const wchar_t* format, ...)
{
	if (!is_trace_needed())
		return 0;

	const size_t buffer_size = buffer.size();
	wchar_t *const buffer_data = buffer.data();

	static std::mutex mutex;
	std::lock_guard<std::mutex> locker(mutex);

	register size_t result = create_prefix(buffer_data, buffer_size);

	switch (category)
	{
	case trace::category::normal:
		break;
	case trace::category::error:
		result += swprintf_s(buffer_data + result, buffer_size - result, L"ERROR> ");
		break;
	}

	va_list args;
	va_start(args, format);
	result += _vswprintf_p(buffer_data + result, buffer_size - result, format, args);
	va_end(args);

	result += swprintf_s(buffer_data + result, buffer_size - result, L"\n");

	if (trace_to.debug_output)
		WinApi::Native::OutputDebugString(buffer_data);
	if (trace_to.file_stream.is_open())
		trace_to.file_stream << buffer_data;

	return result;
}

inline bool tracer_t::is_trace_needed()
{
	return trace_to.debug_output || trace_to.file_stream.is_open();
}

tracer_t::config_t::config_t(size_t buffer_size) :
	buffer_size(buffer_size), 
	trace_to{ nullptr, false } {}

size_t tracer_t::config_t::get_buffer_size() const
{
	return buffer_size;
}

trace::info_t::info_t() :
	date(true), time_msecs(true), id_format(trace::info_t::id_format_t::hex) {}