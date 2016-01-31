#pragma once

#include <vector>
#include "u_scard_wrapper.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef unsigned char byte_t;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MIFARE_ULTRALIGHT__PAGE_COUNT		16
#define MIFARE_ULTRALIGHT__BYTES_PER_PAGE	 4
#define MIFARE_ULTRALIGHT__SIZE_IN_BYTES	(MIFARE_ULTRALIGHT__PAGE_COUNT * MIFARE_ULTRALIGHT__BYTES_PER_PAGE)

#define MIFARE_ULTRALIGHT__SERIAL_NUMBER_SIZE_IN_BYTES		7
#define MIFARE_ULTRALIGHT__MAX_BYTES_READ_ONCE			   16

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace apdu
{
	typedef std::vector<byte_t> content_t;

	class apdu_t
	{
	protected:
		inline virtual ULONG get_size() const = 0;
	};

	class send_t : protected apdu_t
	{
	public:
		send_t(_In_ byte_t cls, _In_ byte_t ins);
		inline ULONG get_size() const;

		struct
		{
			byte_t cls, ins;
			byte_t p[3];
		} packet;

	private:		
	};

	class receive_t : protected apdu_t
	{
	public:
		receive_t();
		bool is_ok() const;
		inline ULONG get_size() const;

		content_t packet;

	private:		
		const byte_t* get_sw() const noexcept;
	};

	namespace error
	{
		class error_t
		{
		public:
			error_t(_In_ const wchar_t *function);

		protected:
			const wchar_t *m_function;
			void output_message(_In_ const wchar_t *message) const;
		};

		class sw_t : public error_t
		{
		public:
			sw_t(_In_ const wchar_t *function, _In_ const byte_t sw[2]);
		private:
		};

		class out_of_range_t : public error_t
		{
		public:
			out_of_range_t(_In_ const wchar_t *function, _In_ size_t value, _In_ size_t upper_bound);
		private:
		};
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class APDU
{
protected:
	APDU(_In_ const CSmartcardManager &SmartcardManager);
	const CSmartcardManager &m_SmartcardManager;
};

class APDU_ReadData : public APDU
{
public:
	APDU_ReadData(_In_ const CSmartcardManager &SmartcardManager);

	ULONG Execute(_In_ byte_t Page, _In_ byte_t BytesToRead) noexcept;
	const apdu::content_t* GetData() const noexcept;

private:
	byte_t GetBytesToRead(_In_ byte_t Page, _In_ byte_t BytesToRead) const;

	apdu::send_t	m_send;
	apdu::receive_t	m_receive;
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------