#pragma once

//#include <vector>
#include <fstream>
#include <mutex>
#include <map>
#include "..\units\u_string.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace trace
{
	// категории трассировки
	enum class category { normal, error };
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// класс трассировщика
class tracer
{
public:

	// конфигуратор трассировщика
	struct config
	{
		// формат выводимых данных
		struct format
		{
			enum class ids { hex = 1, dec = 2 };

			// выводить дату?
			bool date;
			// выводить время с миллисекундами?
			bool time_msecs;
			// выводить номер сессии?
			bool session_id;
			// формат вывода идентификаторов
			ids ids;

			format() noexcept;
		
		};	// struct format

		// куда выводим?
		struct targets
		{
			// вывод в отладочную секцию: туда же, куда и OutputDebugString()
			bool debug;
			// вывод в файл с указанным именем (nullptr или пустая строка, если вывод не требуется)
			cstr_t file;

			targets() noexcept;
		
		};	// struct targets

		format format;							// формат выводимых данных
		targets targets;						// куда выводим данные трассировки

	};	// struct config

	// буффер трассировщика
	class buffer
	{
	public:
		// длина буфера в символах (без учета символа L'\0')
		size_t length() const noexcept;

		buffer(arg_in cstr_t module_name, arg_in const struct config::format &format);
		
		cstr_t c_str() const noexcept;
		const std::vector<char_t>& data() const noexcept;

		void output_string(arg_in trace::category category, arg_in cstr_t format, arg_in ...);
		void output_string(arg_in trace::category category, arg_in cstr_t format, arg_in va_list args);

		void append_string(arg_in cstr_t format, arg_in ...);
		void append_string(arg_in cstr_t format, arg_in va_list args);

	private /*функции-члены*/:
		void create_prefix(arg_in trace::category category);

		void output(arg_in cstr_t format, arg_in ...);
		void output(arg_in cstr_t format, arg_in va_list args);

	private /*данные*/:
		// внутренний буфер, первоначальный размер задается в конструкторе ч/з initial_size
		std::vector<char_t> m__data;
		// указатель на данные внутреннего буфера (хвост), куда будут помещаться новые символы
		str_t m__data_rest;

		// формат выводимых в буфер данных
		const struct config::format m__format;

		// указатель на имя модуля
		const string m__module_name;

	private /*статичные функции*/:
		static constexpr size_t get_initial_size() noexcept;

	private /*статичные данные*/:
		static const std::map<trace::category, cstr_t> &category_prefixes;
	
	};	// static class buffer
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------
	tracer(arg_in cstr_t module_name, arg_in const config &config);
	~tracer();

	//size_t trace(trace::category category, const wchar_t* format, ...);

	size_t trace(arg_in trace::category category, arg_in cstr_t format, arg_in ...);
	
private /*функции-члены*/ :

	// признаки активности таргетов вывода...
	bool is_target__debug() const noexcept;		// в OutputDebugString
	bool is_target__file() const;				// в именованный файл

	// определяет, нужна ли трассировка вообще
	bool is_trace_needed() const;

	// вывод содержимого буфера в таргетты (если они активны)
	void out_buffer_to__debug();				// в OutputDebugString
	void out_buffer_to__file();					// в именованный файл

	void trace_begin();
	void trace_end();

private /*данные*/:
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

	// таргетты вывода (устанавливаются в конструкторе)
	bool m__target_debug;
	std::wofstream m__target_file;
	
	// буфер для вывода
	buffer m__buffer;
	// мютекс для мультипоточного доступа к разделяемым данным буфера
	std::mutex m__mutex;
};