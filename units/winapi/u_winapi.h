#pragma once
#include <windows.h>

namespace WinApi
{
	namespace Native
	{
		inline void WINAPI GetLocalTime(PSYSTEMTIME pTime);
		inline ULONG WINAPI GetCurrentProcessId();
		inline ULONG WINAPI GetCurrentThreadId();
		/*inline*/ void WINAPI OutputDebugString(PCWSTR OutputString);
	}	// namespace Native

	//namespace Wrapper
	//{
		SYSTEMTIME GetLocalTime();
	//}	// namespace Wrapper

		namespace Process
		{
			typedef ULONG ID;

			namespace Current
			{
				ID GetID();
			}
		}

		namespace Thread
		{
			typedef ULONG ID;

			namespace Current
			{
				ID GetID();
			}
		}

}	// namespace WinApi
