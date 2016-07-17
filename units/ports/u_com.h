#pragma once

#include "..\u_system.h"
#include "..\u_exception.h"

namespace ports
{
	class com
	{
	public:
		typedef void* handle_t;
		typedef byte_t number_t;
		
		enum class status { ok = 0/*, все остальные коды - ошибки Windows*/ };
		enum class mode { read = 1, write = 2 };

		class exception;

		com();

		status open(number_t number, size_t mode) noexcept;
		status close() noexcept;

	private:
		handle_t handle;
		inline bool is_opened() const noexcept;
		status close_impl() noexcept;
		status open_impl(number_t number, size_t mode) noexcept;
	};

	class com::exception: public ::exception::base
	{
	public:
	private:
	};
}