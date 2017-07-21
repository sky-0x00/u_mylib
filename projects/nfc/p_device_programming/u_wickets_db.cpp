#include "stdafx.h"

#include "u_wickets_db.h"

#include <map>

using namespace moscow_subway;

static const std::map< station::id, station > stations =
{
	{
		station::id::altufevo, { L"Альтуфьево", { 0x12, 0x56 } }
	},
	{ 
		station::id::bibirevo, { L"Бибирево", { 0x56, 0x6407 } }
	}
};

station::wicket::wicket( 
	_in const wicket &wicket 
) :
	no( wicket.no )
{}
station::wicket::wicket( 
	_in unsigned wicket_no
) :
	no( wicket_no )
{}