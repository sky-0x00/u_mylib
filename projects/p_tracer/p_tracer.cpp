// p_tracer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "..\..\units\u_trace.h"
//#include "..\..\units\u_exception.h"
//#include "..\..\units\u_convert.h"
#include <iostream>
//#include "..\..\units\winapi\u_winapi.h"
#include <cstdarg>

//#include "..\..\units\ui\u_structs.h"
//#include <memory>
//#include <Strsafe.h>

//// Retrieving the Last-Error Code: https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms680582%28v=vs.85%29.aspx
//void GetErrorMessageString(DWORD ErrorCode = GetLastError())
//{
//	LPVOID lpMsgBuf;
//	LPVOID lpDisplayBuf;
//
//	auto free = [](LPVOID *lpMsgBuf)
//	{
//		return WinApi::Native::LocalFree(*lpMsgBuf);
//	};
//
//	std::unique_ptr<LPVOID, decltype(free)> MsgBuf(&lpMsgBuf, free);
//	
//	auto result = WinApi::Native::FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER |
//		FORMAT_MESSAGE_FROM_SYSTEM |
//		FORMAT_MESSAGE_IGNORE_INSERTS,
//		NULL	,
//		ErrorCode,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		reinterpret_cast<LPTSTR>(MsgBuf.get()),
//		//(LPTSTR)&lpMsgBuf,
//		0, NULL);
//
//	// Display the error message and exit the process
//
//	LPTSTR lpszFunction = L"main";
//
//	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
//		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
//	StringCchPrintf((LPTSTR)lpDisplayBuf,
//		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
//		TEXT("%s failed with error %d: %s"),
//		lpszFunction, ErrorCode, lpMsgBuf);
//	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
//
//	//LocalFree(lpMsgBuf);
//	LocalFree(lpDisplayBuf);
//}

int prn(char *s, size_t count, const char *format, ...)
{
	_locale_t loc;
 	va_list args;
	va_start(args, format);
	int res_0 = vsnprintf(s, count, format, args);			// real count (c99)
	int res_1 = _vsnprintf(s, count, format, args);			// -1
	int res_2 = _vsnprintf_l(s, count, format, nullptr, args);		// -1
	va_end(args);
	return res_0;
}
int prn(char_t *s, size_t count, const char_t *format, ...)
{
	va_list args;
	va_start(args, format);
	int res_0 = vswprintf(s, count, format, args);				// -1
	int res_1 = _vswprintf_c(s, count, format, args);			// -1
	int res_2 = _vsnwprintf(s, count, format, args);			// -1
	//int res_3 = vswprintf_s(s, count, format, args);			// ?
	int res_4 = _vsnwprintf_s(s, count, count-1, format, args);
	va_end(args);
	return res_0;
}

int main()
{
	string str0(L"1234");
	//string str0(L"1");
	str0.reverse();
	
	//GetErrorMessageString(5);
	//const auto &s = exception::windows::to_string(0);
	//exception::foo foo_expr();

	//char buf[4];
	//int res = prn(buf, _countof(buf), "11223344");
	//char_t buf[4];
	//int res = prn(buf, _countof(buf), L"11223344");


	tracer::config config;

	config.initial_buffer_size = 0;
	config.targets.debug = true;
	config.targets.file = L"c:\\fout.txt";

	tracer tracer(L"my_module", config);
	tracer.trace(trace::category::normal, L"i: %i", 0x11223344);


	//ui::point_t point1;
	//ui::point_t point2(2, 4);

	//ui::rect_t rect({ 10, 20 }, { 32, 180 });
	//point1.by_width = rect.get_width();
	//point1.by_height = rect.get_height();

	//point2 = rect.get_center_point();
	//point2 = rect.get_center_point(false);


	//tracer_t::config_t config;
	//config.trace_to.debug_output = true;
	//config.trace_to.file_name = L"c:\\output.log";
	//config.info.id = trace::info_t::id_format_t::hex_dec;
	
	//tracer_t tracer(config);
	//tracer.trace(trace::category::error, L"int: %i", 14);
	//tracer.trace(trace::category::error, L"int: %i", 15);

	const byte_t mas[] = { 0x5a, 0x48, 0xF2, 0x0E };
	wchar_t buffer[(sizeof(mas) << 1) + 1];
	//convert::bytes__to__string_hex<sizeof(mas)>(mas, buffer);
	buffer[_countof(buffer) - 1] = L'\0';

	char_t str[] = L"01234";
	//string::reverse(str);

	int08 x = 38;
	//auto n = convert::int32__to__string_dec(x, str);

	//string_t s = convert::bytes__to__string_hex(mas, );

	try
	{
		//exception::out_of_range<int, std::make_tuple(0, 16)>(3);
		//exception::out_of_range<int, 0, 16> e(48);
		//throw e;
	}
	catch (...)
	{
		std::wcout << std::endl;
	}

    return 0;
}