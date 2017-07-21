#pragma once

#include <string>
#include <list>

#include <windows.h>

typedef std::wstring string_t;
typedef std::list<string_t> string_list_t;

class CSmartcardContext
{
public:
	CSmartcardContext();
	~CSmartcardContext();

	operator const SCARDCONTEXT() const;

protected:
	LONG Establish() _NOEXCEPT;
	LONG Release() _NOEXCEPT;

private:
	SCARDCONTEXT m_Context;
};

class CSmartcardError
{
public:
	CSmartcardError(LONG ErrorCode, PCWSTR pszFunction);
private:
	void OutputDebugMessage(LONG ErrorCode, PCWSTR pszFunction) const;
	string_t GetErrorDescription(LONG ErrorCode) const;
};

class CSmartcardMemory
{
public:
	CSmartcardMemory(const CSmartcardContext &CSmartcardContext, LPCVOID pMemoryBlock);
	~CSmartcardMemory();
private:
	const CSmartcardContext &m_CSmartcardContext;
	LPCVOID *m_ppMemoryBlock;
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CSmartcardManager
{
public:
	CSmartcardManager(_In_ const CSmartcardContext &SmartcardContext);
	~CSmartcardManager();

	inline SCARDHANDLE GetCard() const _NOEXCEPT;
	/*inline*/ ULONG GetProtocol() const _NOEXCEPT;

	string_list_t EnumDevices() const;

//protected:
	LONG Connect(
		_In_ PCWSTR pszDeviceName,
		_In_ ULONG ShareMode,
		_In_ ULONG Protocols_Preferred
	) _NOEXCEPT;

	LONG Disconnect(
		_In_ ULONG Disposition
	) _NOEXCEPT;

	LONG Transmit(
		_In_        LPCSCARD_IO_REQUEST pioSendPci,
		_In_        LPCBYTE             pbSendBuffer,
		_In_        DWORD               cbSendLength,
		_Inout_opt_ LPSCARD_IO_REQUEST  pioRecvPci,
		_Out_       LPBYTE              pbRecvBuffer,
		_Inout_     LPDWORD             pcbRecvLength
	) const _NOEXCEPT;

private:
	SCARDHANDLE m_hCard;
	ULONG m_Protocol_Active;

	const CSmartcardContext &m_SmartcardContext;
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------