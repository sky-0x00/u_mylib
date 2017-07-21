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
		//id the_id;								// id ������� (���������� �����)
		string_t name;								// �������� ������� �������������

		struct wicket
		{
			// ������ ���������� ����� �������
			struct list
			{
				station::id station_id;
				std::list< wicket > items;				
			};

			const unsigned no;							// ����� � ������� ������������� (0 - ����� ����������)

			wicket( _in const wicket &wicket );
			wicket( _in unsigned wicket_no );
		};

		std::list< wicket > wickets;				// ������ ���������� ����� �������
	};
	
}	// namespace moscow_subway