#include "stdafx.h"
#include "u_scard_wrapper.h"

#include <sstream>
#include <iomanip>
#include <winscard.h>

typedef unsigned char byte_t;

// -- CSmartcardError helper ----------------------------------------------------------------------------------------------------------------------------------------------------
#define THROW_SCARD_ERROR(error)		throw CSmartcardError(error, __FUNCTIONW__)
#define TRACE_SCARD_ERROR(error)		CSmartcardError(error, __FUNCTIONW__)

//template <typename func_t, typename ...args_t>
//static void TryExec(const func_t &func, args_t&&... args)
//{
//	auto error = func(std::forward<args_t>(args)...);
//	if (SCARD_S_SUCCESS != error)
//		THROW_SCARD_ERROR(error);
//}

// -- CSmartcardContext ----------------------------------------------------------------------------------------------------------------------------------------------------
CSmartcardContext::CSmartcardContext() : m_Context(NULL)
{
	//TryExec(this, Establish);
	auto error = Establish();
	if (SCARD_S_SUCCESS != error)
		THROW_SCARD_ERROR(error);
}
CSmartcardContext::~CSmartcardContext()
{
	if (m_Context)
	{
		//TryExec(this, Release);
		
		//auto error = Release();
		//if (SCARD_S_SUCCESS != error)
		//	THROW_SCARD_ERROR(error);

		Release();
	}
}

LONG CSmartcardContext::Establish() _NOEXCEPT
{
	return ::SCardEstablishContext(SCARD_SCOPE_USER, nullptr, nullptr, &m_Context);
}
LONG CSmartcardContext::Release() _NOEXCEPT
{
	auto error = ::SCardReleaseContext(m_Context);
	if (SCARD_S_SUCCESS == error)
		m_Context = NULL;
	return error;
}

CSmartcardContext::operator const SCARDCONTEXT() const
{
	return m_Context;
}

// -- CSmartcardError ---------------------------------------------------------------------------------------------------------------------------------------------------------
CSmartcardError::CSmartcardError(LONG ErrorCode, PCWSTR pszFunction)
{
	OutputDebugMessage(ErrorCode, pszFunction);
}

void CSmartcardError::OutputDebugMessage(LONG ErrorCode, PCWSTR pszFunction) const
{
	std::wostringstream stream;
	stream << L"exp> func: " << pszFunction << L"(), error: " << std::showbase << std::internal << std::hex << std::setfill(L'0') << std::setw(10) << ErrorCode;
	const string_t &error_description = GetErrorDescription(ErrorCode);
	if (!error_description.empty())
		stream << L" [" << error_description << L"]";
	stream << std::endl;
	OutputDebugString(stream.str().c_str());
}

string_t CSmartcardError::GetErrorDescription(LONG ErrorCode) const
{
	switch (ErrorCode)
	{
		case SCARD_E_NO_READERS_AVAILABLE:
			return L"Cannot find a smart card reader";
		case SCARD_E_CANCELLED:
			return L"The action was canceled by an SCardCancel request";
		case SCARD_E_CANT_DISPOSE:
			return L"The system could not dispose of the media in the requested manner";
		case SCARD_E_CARD_UNSUPPORTED:
			return L"The smart card does not meet minimal requirements for support";
		case SCARD_E_DUPLICATE_READER:
			return L"The reader driver didn't produce a unique reader name";
		case SCARD_E_INSUFFICIENT_BUFFER:
			return L"The data buffer for returned data is too small for the returned data";
		case SCARD_E_INVALID_ATR:
			return L"An ATR string obtained from the registry is not a valid ATR string";
		case SCARD_E_INVALID_HANDLE:
			return L"The supplied handle was invalid";
		case SCARD_E_INVALID_PARAMETER:
			return L"One or more of the supplied parameters could not be properly interpreted";
		case SCARD_E_INVALID_TARGET:
			return L"Registry startup information is missing or invalid";
		case SCARD_E_INVALID_VALUE:
			return L"One or more of the supplied parameter values could not be properly interpreted";
		case SCARD_E_NOT_READY:
			return L"The reader or card is not ready to accept commands";
		case SCARD_E_NOT_TRANSACTED:
			return L"An attempt was made to end a non-existent transaction";
		case SCARD_E_NO_MEMORY:
			return L"Not enough memory available to complete this command";
		case SCARD_E_NO_SERVICE:
			return L"The smart card resource manager is not running";
		case SCARD_E_NO_SMARTCARD:
			return L"The operation requires a smart card, but no smart card is currently in the device";
		case SCARD_E_PCI_TOO_SMALL:
			return L"The PCI receive buffer was too small";
		case SCARD_E_PROTO_MISMATCH:
			return L"The requested protocols are incompatible with the protocol currently in use with the card";
		case SCARD_E_READER_UNAVAILABLE:
			return L"The specified reader is not currently available for use";
		case SCARD_E_READER_UNSUPPORTED:
			return L"The reader driver does not meet minimal requirements for support";
		case SCARD_E_SERVICE_STOPPED:
			return L"The smart card resource manager has shut down";
		case SCARD_E_SHARING_VIOLATION:
			return L"The smart card cannot be accessed because of other outstanding connections";
		case SCARD_E_SYSTEM_CANCELLED:
			return L"The action was canceled by the system, presumably to log off or shut down";
		case SCARD_E_TIMEOUT:
			return L"The user-specified timeout value has expired";
		case SCARD_E_UNKNOWN_CARD:
			return L"The specified smart card name is not recognized";
		case SCARD_E_UNKNOWN_READER:
			return L"The specified reader name is not recognized";
		case SCARD_F_COMM_ERROR:
			return L"An internal communications error has been detected";
		case SCARD_F_INTERNAL_ERROR:
			return L"An internal consistency check failed";
		case SCARD_F_UNKNOWN_ERROR:
			return L"An internal error has been detected, but the source is unknown";
		case SCARD_F_WAITED_TOO_LONG:
			return L"An internal consistency timer has expired";
		case SCARD_W_REMOVED_CARD:
			return L"The smart card has been removed and no further communication is possible";
		case SCARD_W_RESET_CARD:
			return L"The smart card has been reset, so any shared state information is invalid";
		case SCARD_W_UNPOWERED_CARD:
			return L"Power has been removed from the smart card and no further communication is possible";
		case SCARD_W_UNRESPONSIVE_CARD:
			return L"The smart card is not responding to a reset";
		case SCARD_W_UNSUPPORTED_CARD:
			return L"The reader cannot communicate with the card due to ATR string configuration conflicts";
		default:
			return L"";
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSmartcardMemory::CSmartcardMemory(const CSmartcardContext &CSmartcardContext, LPCVOID pMemoryBlock)
	: m_CSmartcardContext(CSmartcardContext), m_ppMemoryBlock((LPCVOID*)(pMemoryBlock))
{ }
CSmartcardMemory::~CSmartcardMemory()
{
	auto error = ::SCardFreeMemory(m_CSmartcardContext, *m_ppMemoryBlock);
	if (SCARD_S_SUCCESS != error)
		TRACE_SCARD_ERROR(error);
	
	*m_ppMemoryBlock = nullptr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSmartcardManager::CSmartcardManager(
	_In_ const CSmartcardContext &SmartcardContext
)
	: m_SmartcardContext(SmartcardContext), m_hCard(NULL), m_Protocol_Active(SCARD_PROTOCOL_UNDEFINED)
{
}

CSmartcardManager::~CSmartcardManager()
{
	if (m_hCard)
		Disconnect(SCARD_UNPOWER_CARD);
}

LONG CSmartcardManager::Connect(
	_In_ PCWSTR pszDeviceName,
	_In_ ULONG ShareMode,
	_In_ ULONG Protocols_Preferred
) _NOEXCEPT
{
	if (m_hCard)
		Disconnect(SCARD_RESET_CARD);

	auto error = ::SCardConnect(m_SmartcardContext, pszDeviceName, ShareMode, Protocols_Preferred, &m_hCard, &m_Protocol_Active);
	if (SCARD_S_SUCCESS != error)
		TRACE_SCARD_ERROR(error);

	return error;
}
LONG CSmartcardManager::Disconnect(
	_In_ ULONG Disposition
) _NOEXCEPT
{
	auto error = ::SCardDisconnect(m_hCard, Disposition);
	if (SCARD_S_SUCCESS == error)
		m_hCard = NULL;
	else
		TRACE_SCARD_ERROR(error);
	return error;
}

inline SCARDHANDLE CSmartcardManager::GetCard() const _NOEXCEPT
{
	return m_hCard;
}
/*inline*/ ULONG CSmartcardManager::GetProtocol() const _NOEXCEPT
{
	return m_Protocol_Active;
}

string_list_t CSmartcardManager::EnumDevices() const
{
	typedef LONG(WINAPI *pfnSCardFreeMemory)(SCARDCONTEXT, LPCVOID);

	LPWSTR pReaders = nullptr;
	DWORD ResultLength = SCARD_AUTOALLOCATE;
	string_list_t readers;

	CSmartcardMemory SmartcardMemory(m_SmartcardContext, &pReaders);

	auto error = ::SCardListReaders(m_SmartcardContext, nullptr, reinterpret_cast<LPWSTR>(&pReaders), &ResultLength);

	if (SCARD_E_NO_READERS_AVAILABLE == error)
		return readers;

	if (SCARD_S_SUCCESS != error)
		THROW_SCARD_ERROR(error);

	// найдено одно или несколько устройств, распарсим мультистринг
	string_t reader;
	for (DWORD i = 0; i < ResultLength; ++i)
	{
		const wchar_t &ch = pReaders[i];
		if (ch)
			reader.push_back(ch);
		else
			if (!reader.empty())
			{
				readers.push_back(reader);
				reader.clear();
			}
	}

	return readers;
}

LONG CSmartcardManager::Transmit(
	_In_        LPCSCARD_IO_REQUEST pioSendPci,
	_In_        LPCBYTE             pbSendBuffer,
	_In_        DWORD               cbSendLength,
	_Inout_opt_ LPSCARD_IO_REQUEST  pioRecvPci,
	_Out_       LPBYTE              pbRecvBuffer,
	_Inout_     LPDWORD             pcbRecvLength
) const _NOEXCEPT 
{
	auto error = ::SCardTransmit(m_hCard, pioSendPci, pbSendBuffer, cbSendLength, pioRecvPci, pbRecvBuffer, pcbRecvLength);
	if (SCARD_S_SUCCESS != error)
		TRACE_SCARD_ERROR(error);
	return error;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------