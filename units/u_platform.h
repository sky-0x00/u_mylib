#pragma once

#define EN__UNSUPPORTED_PLATFORM	L"Unsupported platform"
#define RU__UNSUPPORTED_PLATFORM	L"Неподдерживаемая платформа"

struct platform
{
	enum type
	{
		unknown = 0,
		win32,
		win64		
	};

	static constexpr type get(
	) {
#if defined(_WIN64)
		return win64;
#elif defined(_WIN32)
		return win32;
#else
		return unknown;
#endif
	}

	static constexpr bool is_windows(
	) {
		switch ( platform::get() )
		{
			case win64:
			case win32:
				return true;
			default:
				return false;
		}
	}
};