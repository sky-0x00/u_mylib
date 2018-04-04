#include "stdafx.h"
#include "u_winapi.h"

//******************************************************************************************************************************************
inline void WINAPI WinApi::Native::GetLocalTime(PSYSTEMTIME pTime)
{
	::GetLocalTime(pTime);
}

inline ULONG WINAPI WinApi::Native::GetCurrentProcessId()
{
	return ::GetCurrentProcessId();
}
inline ULONG WINAPI WinApi::Native::GetCurrentThreadId()
{
	return ::GetCurrentThreadId();
}

/*inline*/ void WINAPI WinApi::Native::OutputDebugString(PCWSTR OutputString)
{
	::OutputDebugStringW(OutputString);
}

/*inline*/ BOOL WINAPI WinApi::Native::CloseHandle(_In_ HANDLE hObject)
{
	return ::CloseHandle(hObject);
}

/*inline*/ DWORD WINAPI WinApi::Native::GetLastError()
{
	return ::GetLastError();
}
inline void WINAPI WinApi::Native::SetLastError(DWORD ErrorCode)
{
	return ::SetLastError(ErrorCode);
}

/*inline*/ DWORD WINAPI WinApi::Native::FormatMessage(
	_In_     DWORD   Flags,
	_In_opt_ LPCVOID Source,
	_In_     DWORD   MessageId,
	_In_     DWORD   LanguageId,
	_Out_    LPTSTR  Buffer,
	_In_     DWORD   Size,
	_In_opt_ va_list *Arguments
) {
	return ::FormatMessageW(Flags, Source, MessageId, LanguageId, Buffer, Size, Arguments);
}

/*inline*/ HLOCAL WINAPI WinApi::Native::LocalFree(_In_ HLOCAL hMemory)
{
	return ::LocalFree(hMemory);
}

inline HANDLE WINAPI WinApi::Native::CreateFile(
	_In_     LPCTSTR               pFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES pSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
) {
	return ::CreateFileW(pFileName, dwDesiredAccess, dwShareMode, pSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

inline BOOL WinApi::Native::ProcessIdToSessionId(
	_In_  DWORD dwProcessId,
	_Out_ DWORD *pSessionId
) {
	return ::ProcessIdToSessionId(dwProcessId, pSessionId);
}

//******************************************************************************************************************************************
SYSTEMTIME WinApi::GetLocalTime()
{
	SYSTEMTIME Time;
	Native::GetLocalTime(&Time);
	return Time;
}

//------------------------------------------------------------------------------------------------------------------------------------------
WinApi::Process::Id WinApi::Process::Current::Id()
{
	return WinApi::Native::GetCurrentProcessId();
}
WinApi::Thread::Id WinApi::Thread::Current::Id()
{
	return WinApi::Native::GetCurrentThreadId();
}
WinApi::Session::Id WinApi::Session::Current::Id(Process::Id Id)
{
	DWORD Result;
	if (Native::ProcessIdToSessionId(Id, &Result))
		return Result;

	return WinApi::Session::Id::Unknown;
}


//******************************************************************************************************************************************
WinApi::LastErrorGuard::LastErrorGuard()
{
	LastError = WinApi::Native::GetLastError();
}
WinApi::LastErrorGuard::~LastErrorGuard()
{
	WinApi::Native::SetLastError(LastError);
}

//------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ const WinApi::Session::Id WinApi::Session::Id::Unknown = MAXDWORD;

WinApi::Session::Id::Id(DWORD Id) :
	m_Id(Id) {}

//WinApi::Session::Id::operator DWORD() const
//{
//	return m_Id;
//}
bool WinApi::Session::Id::operator==(const WinApi::Session::Id &Id) const
{
	return m_Id == Id.m_Id;
}
bool WinApi::Session::Id::operator!=(const WinApi::Session::Id &Id) const
{
	return m_Id != Id.m_Id;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------