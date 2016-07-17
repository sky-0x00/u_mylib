#pragma once

//#include <utility>
//#include <tuple>

#include "u_string.h"
#include "winapi\u_winapi.h"
#include "winapi\u_types.h"
#include "u_convert.h"
#include "u_string.h"

#include <array>

// если объ€влено TRACE_EXCEPTION, то OutputDebugString() трассируетс€ информаци€ об ошибке

namespace exception
{
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class base
	{
	public:
		base() noexcept;
		explicit base(const char_t* message) noexcept;
		base(const base& other) noexcept;

		virtual ~base() noexcept;

		base& operator=(const base& other) noexcept;
		const char_t* get_message() const noexcept;

	protected:
		void set_message(arg_in cstr_t message) noexcept;
		virtual void set_message() noexcept;

	private:
		string m_message;
	};

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	template <typename type_t>
	class out_of_range : public base
	{
	private:
		
	public:
		//explicit out_of_range(const type_t &value, const std::array<type_t, 2> &range) noexcept
		explicit out_of_range(const type_t &value, const type_t (&range)[2]) noexcept
			: value(value), range(range) {};
		
	private:
		virtual void set_message() noexcept;
		
		const type_t &value;
		//const std::array<type_t, 2> &range;
		const type_t (&range)[2];
	};

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// System Error Codes: https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms681381%28v=vs.85%29.aspx
	class windows: public base
	{
	public:
		windows(dword_t e_code = WinApi::Native::GetLastError(), module_t h_module = nullptr) noexcept;

		// возвращает строковое продставление об ошибке с кодом e_code из модул€ h_module или из системы, если модуль не указан
		// Retrieving the Last-Error Code: https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms680582%28v=vs.85%29.aspx
		static string to_string(dword_t e_code, module_t h_module = nullptr) noexcept;

	private:
		virtual void set_message() noexcept;

		dword_t  e_code;
		module_t h_module;
	};

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

}	// namespace exeption