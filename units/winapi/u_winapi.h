#pragma once
#include <windows.h>

#ifdef OutputDebugString
	#undef OutputDebugString
#endif

#ifdef FormatMessage
	#undef FormatMessage
#endif

#ifdef CreateFile
	#undef CreateFile
#endif

namespace WinApi
{
	namespace Native
	{
		inline void WINAPI GetLocalTime(PSYSTEMTIME pTime);
		inline ULONG WINAPI GetCurrentProcessId();
		inline ULONG WINAPI GetCurrentThreadId();
		/*inline*/ void WINAPI OutputDebugString(_In_ PCWSTR OutputString);
		/*inline*/ BOOL WINAPI CloseHandle(_In_ HANDLE hObject);
		/*inline*/ DWORD WINAPI GetLastError();
		inline void WINAPI SetLastError(DWORD ErrorCode);
		/*inline*/ DWORD WINAPI FormatMessage(
			_In_     DWORD   Flags,
			_In_opt_ LPCVOID Source,
			_In_     DWORD   MessageId,
			_In_     DWORD   LanguageId,
			_Out_    LPTSTR  Buffer,
			_In_     DWORD   Size,
			_In_opt_ va_list *Arguments
		);
		inline HANDLE WINAPI CreateFile(
			_In_     LPCTSTR               pFileName,
			_In_     DWORD                 dwDesiredAccess,
			_In_     DWORD                 dwShareMode,
			_In_opt_ LPSECURITY_ATTRIBUTES pSecurityAttributes,
			_In_     DWORD                 dwCreationDisposition,
			_In_     DWORD                 dwFlagsAndAttributes,
			_In_opt_ HANDLE                hTemplateFile
		);

		/*inline*/ HLOCAL WINAPI LocalFree(_In_ HLOCAL hMemory);

		inline BOOL ProcessIdToSessionId(
			_In_  DWORD dwProcessId,
			_Out_ DWORD *pSessionId
		);

		namespace Type
		{
			typedef HLOCAL (WINAPI *LocalFree)(_In_ HLOCAL);
		}

	}	// namespace Native

	//namespace Wrapper
	//{
		SYSTEMTIME GetLocalTime();
	//}	// namespace Wrapper

		namespace Process
		{
			typedef DWORD Id;

			namespace Current
			{
				Process::Id Id();
			}
		}

		namespace Thread
		{
			typedef DWORD Id;

			namespace Current
			{
				Thread::Id Id();
			}
		}

		namespace Session
		{
			class Id
			{
			public:
				static const Id Unknown;
				
				Id(DWORD Id);
				bool operator==(const Id &Id) const;
				bool operator!=(const Id &Id) const;
			//	operator DWORD() const;
			private:
				DWORD m_Id;
			};

			namespace Current
			{
				Session::Id Id(Process::Id Id = Process::Current::Id());
			}
		}

		class LastErrorGuard
		{
		public:
			LastErrorGuard();
			~LastErrorGuard();
		private:
			DWORD LastError;
		};

}	// namespace WinApi
