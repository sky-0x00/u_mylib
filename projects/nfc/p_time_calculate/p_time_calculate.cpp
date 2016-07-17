// p_time_calculate.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <locale.h>
#include <string>

typedef unsigned char byte_t;
typedef unsigned short uint16_t;

struct st_time_t
{
	byte_t h, m, s;
};

void get_time(_In_ uint16_t time_pack, _Out_ st_time_t &time)
{
	const static size_t utc_h = 3;

	double result = 24.0 * time_pack / 0x10000;
	time.h = static_cast<byte_t>(result);
	result = 60.0 * (result - time.h);
	time.m = static_cast<byte_t>(result);
	result = 60.0 * (result - time.m);
	time.s = static_cast<byte_t>(result);

	time.h += utc_h;
}

int main()
{
	{
		const wchar_t *pszLocaleName = L"Russian_Russia.866";
		if (!_wsetlocale(LC_ALL, pszLocaleName))
			wprintf(L"error: _wsetlocale(%s) failed", pszLocaleName);
	}

	uint16_t time_packs[] = { 0x4dc8, 0x4d88, 0x9a08, 0x90c8 };
	for (auto time_pack : time_packs)
	{
		st_time_t time;
		get_time(time_pack, time);
		wprintf(L"time: 0x%04X, %02u:%02u:%02u\n", time_pack, time.h, time.m, time.s);
	}

    return 0;
}

