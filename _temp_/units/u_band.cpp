#include "stdafx.h"
#include "u_band.h"

tc_band_f::tc_band_f() {
	// ничего не делаем
}

void tc_band_f::set(type amin, type apos, type amax) {
	fmin = amin;
	fpos = apos;
	fmax = amax;
}
type tc_band_f::get_min() {
	return fmin;
}
type tc_band_f::get_pos() {
	return fpos;
}
type tc_band_f::get_max() {
	return fmax;
}
void tc_band_f::set_pos(type apos) {
	fpos = apos;
}
real_s tc_band_f::get_fraction_percent() {
	return 100 * get_fraction();
}
real_s tc_band_f::get_fraction() {
	return (real_s)(fpos - fmin) / (real_s)(fmax - fmin);
}

tc_band_s::tc_band_s(): tc_band_f() {
}
void tc_band_s::set(type amin, type apos, type amax) {
	if (amax < amin) 
		throw -1;	//"значение правой границы диапазона значений должно быть не меньше значения левой"
	if ((apos < amin) || (apos > amax)) 
		throw -2;	//"значение позиции должно быть в пределах диапазона [amin, amax]"
	tc_band_f::set(amin, apos, amax);
}
void tc_band_s::set_pos(type apos) {
	if ((apos < fmin) || (apos > fmax))
		throw -2;	//"значение позиции должно быть в пределах диапазона [amin, amax]"
	tc_band_f::set_pos(apos);
}

tc_progressbar_cli_f::tc_progressbar_cli_f() {
	band = new tc_band_f;
	flength = 40;
}
tc_progressbar_cli_f::~tc_progressbar_cli_f() {
	delete band;
}

uint08 round(real_s x) {  // только положит числа
	uint08 res = x;
	return (x - res) < 0.5 ? res : res + 1;
}
uint08 tc_progressbar_cli_f::get_length_elapse() {
	return round(flength * band->get_fraction());
}
void tc_progressbar_cli_f::draw() {
	//#define ch_elapse 219
	#define ch_elapse 178
	#define ch_remain 176
	uint08 length_elapse = get_length_elapse(),
		   i = 0;
	for (; i < length_elapse; i++)
		putchar((char_t)ch_elapse);
	for (; i < flength; i++)
		putchar((char_t)ch_remain);
	#undef ch_elapse
	#undef ch_remain
}