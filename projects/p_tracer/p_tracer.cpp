// p_tracer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "..\..\units\u_tracer.h"


int main()
{
	tracer_t::config_t config;
	//config.trace_to.debug_output = true;
	//config.trace_to.file_name = L"c:\\output.log";
	config.info.id_format = trace::info_t::id_format_t::hex_dec;
	
	tracer_t tracer(config);
	tracer.trace(trace::category::error, L"int: %i", 14);
	tracer.trace(trace::category::error, L"int: %i", 15);

    return 0;
}

