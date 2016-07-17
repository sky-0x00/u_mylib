// p_codepage.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#define ROWS 8

int _tmain(int argc, _TCHAR* argv[]) {
	for (unsigned short i = 32; i < 256; ) {
		for (unsigned char j = 0; j < ROWS; j++, i++) {
			printf("%3i: %c\t", i, i);
			if (i == 256)
				break;
		}
		printf("\n");
	}
	printf("press [esc] for exit...");
	while (getch() != 27);
	return 0;
}

