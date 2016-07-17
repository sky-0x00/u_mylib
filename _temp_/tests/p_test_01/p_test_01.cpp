// p_test_01.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "..\..\units\u_math.h"
#include "..\..\units\u_band.h"
#include <Windows.h>

HANDLE hOuput = 0;

void printf_band_values(tc_band_f *band) {
	printf("min: %i, pos: %i, max: %i  \n", band->min, band->pos, band->max);
}
void printf_band_percent(tc_band_f *band) {
	printf(" %5.1f%% | ", band->get_fraction_percent());
}

void gotoxy(int xpos, int ypos) {
	COORD scrn;
	if (!hOuput) 
		hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput,scrn);
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*if (!hOuput) 
		hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CCI;
	GetConsoleCursorInfo(hOuput, &CCI);
	CCI.bVisible = 0;
	SetConsoleCursorInfo(hOuput, &CCI);

	tc_progressbar_cli_f *progressbar = new tc_progressbar_cli_f;
	//tc_band_f *band = new tc_band_f;
	//tc_band_s *band = new tc_band_s;
	progressbar->band->set(0, 0, 30);
	printf("\n\nнажмите [esc] для выхода...\n");
	char_t ch, is_draw = 1;
	do {
		if (is_draw) {
			is_draw = 0;
			gotoxy(0, 0);
			progressbar->draw();
			printf_band_percent(progressbar->band);
			printf_band_values(progressbar->band);
		}
		ch = getch();
		if (ch == 224)	// управляющая клавиша
			ch = getch();
		switch (ch) {
			case 75:	// left
				if (progressbar->band->pos > progressbar->band->min) {
					progressbar->band->pos--;
					is_draw = 1;
				}
				break;
			case 77:	// right
				if (progressbar->band->pos < progressbar->band->max) {
					progressbar->band->pos++;
					is_draw = 1;
				}
				break;
		}	// switch
	} while (ch != 27);		// esc

	//printf_band_values(progressbar->band);
	//progressbar->draw();

	delete progressbar;
	*/

	

	int08 x = 1;
	printf("x: %i, abs(x): %i\n", x, math::abs(x));
	x = -127;
	printf("x: %i, abs(x): %i\n", x, math::abs(x));
	x = 127;
	printf("x: %i, abs(x): %i\n", x, math::abs(x));
	x = 128;
	printf("x: %i, abs(x): %i\n", x, math::abs(x));
	x = -128;
	printf("x: %i, abs(x): %i\n", x, math::abs(x));

	printf("нажмите [esc] для выхода...");
	while (getch() != 27);
	return 0;
}

