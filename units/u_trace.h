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

		size_t initial_buffer_size;				// первоначальный размер буфера трассировщика
		format format;							// формат выводимых данных
		targets targets;						// куда выводим данные трассировки

		config() noexcept;
	
	};	// struct config

	// буффер трассировщика, мьютекс лочится снаружи
	static class buffer
	{
	public:
		void create_prefix(arg_in const struct config::format &format);
		void append_string(arg_in trace::category category, arg_in cstr_t format, arg_in va_list args);

		// длина заполненной части буфера в символах (без учета символа L'\0')
		size_t get_length() const noexcept;

		std::mutex& get_mutex() noexcept;
		cstr_t get_data() const noexcept;

		buffer(const string &module_name, size_t initial_size);

	private:
		void output(cstr_t format, ...);
		void output(cstr_t format, va_list args);

		// внутренний буфер, первоначальный размер задается в конструкторе ч/з initial_size
		std::vector<char_t> m_data;
		// указатель на данные внутреннего буфера (хвост), куда будут помещаться новые символы
		str_t m_newdata_ptr;

		// мютекс для мультипоточного доступа к разделяемым данным извне
		std::mutex m_mutex;

		// указатель на имя модуля, хранимое в классе tracer
		const string &m_module_name;

		static const std::map<trace::category, cstr_t> &category_prefixes;
	
	};	// static class buffer
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------
	tracer(cstr_t module_name, const config &config);
	~tracer();

	// геттеры для получения таргетов в OutputDebugString и именованный файл
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

	// имя модуля
	const string m_name;

	// таргетты вывода
	bool m_target_debug;
	std::wofstream m_fstream;
	
	// буфер для вывода
	buffer m_buffer;

	struct config::format m_format;

	void out_buffer_to__debug() const;

	size_t buffer__create_prefix();
	size_t buffer__append_string(const char_t *format, ...);
};