#pragma once

#include <vector>
#include <fstream>
#include <mutex>
#include <map>
//#include "..\units\u_system.h"
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

		size_t initial_buffer_size;				// �������������� ������ ������ �������������
		format format;							// ������ ��������� ������
		targets targets;						// ���� ������� ������ �����������

		config() noexcept;
	
	};	// struct config

	// ������ �������������, ������� ������� �������
	static class buffer
	{
	public:
		void create_prefix(arg_in const struct config::format &format);
		void append_string(arg_in trace::category category, arg_in cstr_t format, arg_in va_list args);

		// ����� ����������� ����� ������ � �������� (��� ����� ������� L'\0')
		size_t get_length() const noexcept;

		std::mutex& get_mutex() noexcept;
		cstr_t get_data() const noexcept;

		buffer(const string &module_name, size_t initial_size);

	private:
		void output(cstr_t format, ...);
		void output(cstr_t format, va_list args);

		// ���������� �����, �������������� ������ �������� � ������������ �/� initial_size
		std::vector<char_t> m_data;
		// ��������� �� ������ ����������� ������ (�����), ���� ����� ���������� ����� �������
		str_t m_newdata_ptr;

		// ������ ��� ��������������� ������� � ����������� ������ �����
		std::mutex m_mutex;

		// ��������� �� ��� ������, �������� � ������ tracer
		const string &m_module_name;

		static const std::map<trace::category, cstr_t> &category_prefixes;
	
	};	// static class buffer
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------
	tracer(cstr_t module_name, const config &config);
	~tracer();

	// ������� ��� ��������� �������� � OutputDebugString � ����������� ����
	bool is_target__debug() const;
	bool is_target__file() const;

	bool is_trace_needed() const;

	//~tracer();
	//size_t trace(trace::category category, const wchar_t* format, ...);

	size_t trace(trace::category category, cstr_t format, ...);

private:
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

	// ��� ������
	const string m_name;

	// �������� ������
	bool m_target_debug;
	std::wofstream m_fstream;
	
	// ����� ��� ������
	buffer m_buffer;

	struct config::format m_format;

	void out_buffer_to__debug() const;

	size_t buffer__create_prefix();
	size_t buffer__append_string(const char_t *format, ...);
};