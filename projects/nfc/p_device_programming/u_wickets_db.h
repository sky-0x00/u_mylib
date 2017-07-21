#pragma once

#include "stdafx.h"
#include <list>

namespace moscow_subway
{
	struct station
	{
		enum class id
		{
			altufevo,
			bibirevo
		};
		//id the_id;								// id станции (уникальный номер)
		string_t name;								// название станции метрополитена

		struct wicket
		{
			// список турникетов одной станции
			struct list
			{
				station::id station_id;
				std::list< wicket > items;				
			};

			const unsigned no;							// номер в системе метрополитена (0 - номер неизвестен)

			wicket( _in const wicket &wicket );
			wicket( _in unsigned wicket_no );
		};

		std::list< wicket > wickets;				// список турникетов одной станции
	};
	
}	// namespace moscow_subway