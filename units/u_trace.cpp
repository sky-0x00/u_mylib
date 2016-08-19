#include "stdafx.h"

#include <cstdarg>
#include <ctime>
//#include <fstream>

#include "u_trace.h"
#include "winapi\u_winapi.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// приращение размера буфера, если текуший размер недостаточен
#define BUFFER_INC_SIZE					32

// ошибка...
#define BUFFER_CALC_LENGTH_ERROR		L"ошибка вычисления размера буфера трассировки"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ const std::map<trace::category, cstr_t> &tracer::buffer::category_prefixes =
{
	{ trace::category::normal, L"" },
	{ trace::category::error,  L"E> " }
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
tracer::tracer(
	arg_in cstr_t module_name, 
	arg_in const config &config
) : 
	m__buffer(module_name, config.format)
{
	m__target_debug = config.targets.debug;
	if (string::check(config.targets.file))
		m__target_file.open(config.targets.file, std::ios_base::out | std::ios_base::app);
	
	trace_begin();
}

tracer::~tracer()
{
	trace_end();
}

bool tracer::is_target__debug() const noexcept
{
	return m__target_debug;
}
bool tracer::is_target__file() const
{
	return m__target_file.is_open();
}
bool tracer::is_trace_needed() const
{
	return is_target__debug() || is_target__file();
}

void tracer::out_buffer_to__debug()
{
	if (!is_target__debug())
		return;

	WinApi::Native::OutputDebugString(m__buffer.c_str());
	WinApi::Native::OutputDebugString(string::special::newline());
}
void tracer::out_buffer_to__file()
{
	if (!is_target__file())
		return;

	m__target_file << m__buffer.c_str() << string::special::newline();
}

void tracer::trace_begin()
{
	if (!is_trace_needed())
		return;

	std::lock_guard<std::mutex> mutex_locker(m__mutex);
	m__buffer.output_string(
		trace::category::normal, 
		L"--------------------------------------------------------------------------------------<<--"
	);

	out_buffer_to__debug();
	out_buffer_to__file();
}
void tracer::trace_end()
{
	if (!is_trace_needed())
		return;

	std::lock_guard<std::mutex> mutex_locker(m__mutex);
	m__buffer.output_string(
		trace::category::normal, 
		L"-------------------------------------------------------------------------------------->>--"
	);

	out_buffer_to__debug();
	m__buffer.append_string(string::special::newline());
	out_buffer_to__file();
}

size_t tracer::trace(
	arg_in trace::category category,
	arg_in cstr_t format,
	arg_in ...
) {
	if (!is_trace_needed())
		return 0;

	va_list args;
	va_start(args, format);
	
	std::lock_guard<std::mutex> mutex_locker(m__mutex);
	m__buffer.output_string(category, format, args);

	out_buffer_to__debug();
	out_buffer_to__file();
	
	va_end(args);
	
	return m__buffer.length();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void tracer::buffer::output(
	arg_in cstr_t format,
	arg_in va_list args
) {
	const size_t length = this->length();

	// m__data_rest - остаточный буфер, он лежит в диапазоне [m__data.data(); m__data.data() + m__data.size())
	// число символов в остаточном буфере m__data_rest равно rest_size	
	const size_t rest_size = m__data.size() - length;			// с учетом символа '\0'

	// выводим форматированную строку в буфер m__data_rest
	auto count = vswprintf(m__data_rest, rest_size, format, args);
	if (-1 == count)
	{
		// размер буфера в rest_size недостаточен, переустановим его
		count = _vscwprintf(format, args);
		if (-1 == count)
			throw BUFFER_CALC_LENGTH_ERROR;
	}
	if (count >= rest_size)
	{
		m__data.resize(++count + length);
		m__data_rest = m__data.data() + length;
		count = vswprintf(m__data_rest, count, format, args);
		if (-1 == count)
			throw BUFFER_CALC_LENGTH_ERROR;
	}
	m__data_rest += count;
}

void tracer::buffer::output(
	arg_in cstr_t format,
	arg_in ...
) {
	va_list args;
	va_start(args, format);
	output(format, args);
	va_end(args);
}

tracer::buffer::buffer(
	arg_in cstr_t module_name,
	arg_in const struct config::format &format
) :
	m__module_name(module_name),
	m__data(get_initial_size()),
	m__data_rest(m__data.data()),
	m__format(format)
{}

void tracer::buffer::create_prefix(
	arg_in trace::category category
) {
	m__data_rest = m__data.data();

	SYSTEMTIME Time;
	WinApi::Native::GetLocalTime(&Time);
	
	// выводим отметку времени
	if (m__format.date)
	{
		// "YYYY-MM-DD "
		output(L"%04u-%02u-%02u ", Time.wYear, Time.wMonth, Time.wDay);
	}
	output(L"%02u:%02u:%02u", Time.wHour, Time.wMinute, Time.wSecond);
	if (m__format.time_msecs)
	{
		// ".MSMS"
		output(L".%03u", Time.wMilliseconds);
	}
	
	// выводим имя модуля
	output(L" [%s]", m__module_name.c_str());

	// формируем информацию о сессии
	char_t SessionId_String[16] = L"";
	if (m__format.session_id)
	{
		const auto SessionId = WinApi::Session::Current::Id();
		if (SessionId == WinApi::Session::Id::Unknown)
			wcscpy_s(SessionId_String, L"? ");
		else
			switch (m__format.ids)
			{
			case config::format::ids::dec:
				swprintf_s(SessionId_String, L"%u ", SessionId);
				break;
			case config::format::ids::hex:
				swprintf_s(SessionId_String, L"0x%X ", SessionId);
				break;
			}
	}

	// выводим информацию о сессии + идентификаторы [процесса:потока]
	switch (m__format.ids)
	{
	case config::format::ids::dec:
		output(L" [%s%u:%u]", SessionId_String, WinApi::Process::Current::Id(), WinApi::Thread::Current::Id());
		break;
	case config::format::ids::hex:
		output(L" [%s0x%04X:0x%04X]", SessionId_String, WinApi::Process::Current::Id(), WinApi::Thread::Current::Id());
		break;
	}

	// выводим символы начала (приглашение на вывод)
	output(L" %s", category_prefixes.at(category));
}

void tracer::buffer::append_string(
	arg_in cstr_t format,
	arg_in va_list args
) {
	// выводим переданную user'ом трассировочную строку
	output(format, args);
}

void tracer::buffer::append_string(
	arg_in cstr_t format, 
	arg_in ...
) {
	va_list args;
	va_start(args, format);
	append_string(format, args);
	va_end(args);
}

void tracer::buffer::output_string(
	arg_in trace::category category,
	arg_in cstr_t format, 
	arg_in va_list args
) {
	create_prefix(category);
	append_string(format, args);
}

void tracer::buffer::output_string(
	arg_in trace::category category,
	arg_in cstr_t format,
	arg_in ...
) {
	va_list args;
	va_start(args, format);
	output_string(category, format, args);
	va_end(args);
}

size_t tracer::buffer::length() const noexcept
{
	//assert(m__data.size())
	//assert(m__data_rest >= m__data.data())
	//assert(m__data_rest < m__data.data() + m__data.size())

	return m__data_rest - m__data.data();
}
cstr_t tracer::buffer::c_str() const noexcept
{
	return m__data.data();
}
const std::vector<char_t>& tracer::buffer::data() const noexcept
{
	return m__data;
}

/*static*/ constexpr size_t tracer::buffer::get_initial_size() noexcept
{
	return BUFFER_INC_SIZE << 4;
}

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

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
