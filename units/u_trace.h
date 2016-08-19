#pragma once

//#include <vector>
#include <fstream>
#include <mutex>
#include <map>
#include "..\units\u_string.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace trace
{
	// ��������� �����������
	enum class category { normal, error };
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����� �������������
class tracer
{
public:

	// ������������ �������������
	struct config
	{
		// ������ ��������� ������
		struct format
		{
			enum class ids { hex = 1, dec = 2 };

			// �������� ����?
			bool date;
			// �������� ����� � ��������������?
			bool time_msecs;
			// �������� ����� ������?
			bool session_id;
			// ������ ������ ���������������
			ids ids;

			format() noexcept;
		
		};	// struct format

		// ���� �������?
		struct targets
		{
			// ����� � ���������� ������: ���� ��, ���� � OutputDebugString()
			bool debug;
			// ����� � ���� � ��������� ������ (nullptr ��� ������ ������, ���� ����� �� ���������)
			cstr_t file;

			targets() noexcept;
		
		};	// struct targets

		format format;							// ������ ��������� ������
		targets targets;						// ���� ������� ������ �����������

	};	// struct config

	// ������ �������������
	class buffer
	{
	public:
		// ����� ������ � �������� (��� ����� ������� L'\0')
		size_t length() const noexcept;

		buffer(arg_in cstr_t module_name, arg_in const struct config::format &format);
		
		cstr_t c_str() const noexcept;
		const std::vector<char_t>& data() const noexcept;

		void output_string(arg_in trace::category category, arg_in cstr_t format, arg_in ...);
		void output_string(arg_in trace::category category, arg_in cstr_t format, arg_in va_list args);

		void append_string(arg_in cstr_t format, arg_in ...);
		void append_string(arg_in cstr_t format, arg_in va_list args);

	private /*�������-�����*/:
		void create_prefix(arg_in trace::category category);

		void output(arg_in cstr_t format, arg_in ...);
		void output(arg_in cstr_t format, arg_in va_list args);

	private /*������*/:
		// ���������� �����, �������������� ������ �������� � ������������ �/� initial_size
		std::vector<char_t> m__data;
		// ��������� �� ������ ����������� ������ (�����), ���� ����� ���������� ����� �������
		str_t m__data_rest;

		// ������ ��������� � ����� ������
		const struct config::format m__format;

		// ��������� �� ��� ������
		const string m__module_name;

	private /*��������� �������*/:
		static constexpr size_t get_initial_size() noexcept;

	private /*��������� ������*/:
		static const std::map<trace::category, cstr_t> &category_prefixes;
	
	};	// static class buffer
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------
	tracer(arg_in cstr_t module_name, arg_in const config &config);
	~tracer();

	//size_t trace(trace::category category, const wchar_t* format, ...);

	size_t trace(arg_in trace::category category, arg_in cstr_t format, arg_in ...);
	
private /*�������-�����*/ :

	// �������� ���������� �������� ������...
	bool is_target__debug() const noexcept;		// � OutputDebugString
	bool is_target__file() const;				// � ����������� ����

	// ����������, ����� �� ����������� ������
	bool is_trace_needed() const;

	// ����� ����������� ������ � �������� (���� ��� �������)
	void out_buffer_to__debug();				// � OutputDebugString
	void out_buffer_to__file();					// � ����������� ����

	void trace_begin();
	void trace_end();

private /*������*/:
	//void initialize(const config_t &config);

	//size_t create_prefix(wchar_t *buffer_data, size_t buffer_size);
	//
	//std::vector<wchar_t> buffer;
	//
	//struct
	//{
	//	std::wofstream file_stream;
	//	bool debug_output;
	//} trace_to;

	//trace::info_t info;
	//inline bool is_trace_needed();

	// �������� ������ (��������������� � ������������)
	bool m__target_debug;
	std::wofstream m__target_file;
	
	// ����� ��� ������
	buffer m__buffer;
	// ������ ��� ��������������� ������� � ����������� ������ ������
	std::mutex m__mutex;
};