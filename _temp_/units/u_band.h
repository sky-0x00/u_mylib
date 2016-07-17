#include "u_types.h"
//#include "stdafx.h"

#define type uint08

class tc_band_f { 
    protected:
        type fmin, fmax, fpos; 
		
    public:
		tc_band_f();
		void set(type amin, type apos, type amax);       
		real_s get_fraction_percent();
		real_s get_fraction();

		type get_min();		
		type get_max();		virtual void set_pos(type apos);
		type get_pos();		
		__declspec(property(get = get_min))                type min;
		__declspec(property(get = get_pos, put = set_pos)) type pos;
		__declspec(property(get = get_max))                type max; 
};

class tc_band_s: public tc_band_f  { 
    public:
		tc_band_s();
		void set(type amin, type apos, type amax);
		void set_pos(type apos);		
};

class tc_progressbar_cli_f {
	protected:
		uint08 flength;		// длина в символах экрана
		uint08 col_attr;	// атрибут цвета (цвет символа + цвет заливки)
		uint08 get_length_elapse();
	public:
		tc_band_f *band;
		
		tc_progressbar_cli_f();
		~tc_progressbar_cli_f();

		void draw();

};