#pragma once

#include <vector>
#include <fstream>
#include "..\units\u_system.h"

//enum class 

namespace trace
{
	const wchar_t separator = L' ';
	enum class category { normal, error };

	struct default
	{
		const static size_t buffer_size = 0x200;
	};
	struct info_t
	{
		enum class id_format_t { hex = 1, dec = 2, hex_dec = 3 };
		
		bool date;
		bool time_msecs;
		id_format_t id_format;

		info_t();
	};
}

class tracer_t
{
public:
	class config_t;
	
	tracer_t(const config_t &config);
	~tracer_t();
	size_t trace(trace::category category, const wchar_t* format, ...);

private:
	void initialize(const config_t &config);

	size_t create_prefix(wchar_t *buffer_data, size_t buffer_size);
	
	std::vector<wchar_t> buffer;
	
	struct
	{
		std::wofstream file_stream;
		bool debug_output;
	} trace_to;

	trace::info_t info;
	inline bool is_trace_needed();
};

class tracer_t::config_t
{
public:
	struct info_t;

	struct
	{
		const wchar_t *file_name;
		bool debug_output;
	} trace_to;

	trace::info_t info;

	config_t(size_t buffer_size = trace::default::buffer_size);
	size_t get_buffer_size() const;

private:
	size_t buffer_size;
};