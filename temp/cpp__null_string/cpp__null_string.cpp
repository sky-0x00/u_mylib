// cpp__null_string.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <string>

int main()
{
	wchar_t *p = L"12345";
	std::wstring s(0, L'*');

    return 0;
}

