// p_device_programming.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "u_device.h"

int main()
{
	try
	{
		smartcard::context_t context;
		smartcard::device_t device(context);

		auto device_names = device.enum_all();
		if (device_names.empty())
			return -1;

		device.connect(device_names.front().c_str());

		apdu::content_t content;
		content.reserve(MIFARE_ULTRALIGHT__SIZE_IN_BYTES);

		device.read_data(0, 4, content);		
	}
	catch (...)
	{
		return -1;
	}

    return 0;
}

