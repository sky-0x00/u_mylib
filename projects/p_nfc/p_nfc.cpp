// p_nfc.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "..\..\units\windows\u_smartcard.h"
#include "..\..\units\windows\winapi\u_winapi.h"

int main()
{
	try 
	{
		smartcard::manager smgr;
		smgr.enum_devices();

	} catch (Winapi::Error e)
	{
		printf( "error: %i (0x%08X)\n", e, e );
	}
	catch (...)
	{
		printf( "error: unknown\n" );
	}
    return 0;
}

