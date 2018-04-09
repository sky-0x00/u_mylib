#pragma once

#define PLATFORM_UNSUPPORTED__EN	L"Unsupported platform"
#define PLATFORM_UNSUPPORTED__RU	L"Неподдерживаемая платформа"

// пока так
#define PLATFORM_UNSUPPORTED		PLATFORM_UNSUPPORTED__EN

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