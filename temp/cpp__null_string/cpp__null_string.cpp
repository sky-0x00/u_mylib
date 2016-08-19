// cpp__null_string.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>

int main()
{
	wchar_t *p = L"12345";
	std::wstring s(0, L'*');

    return 0;
}

