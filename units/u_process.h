#pragma once

namespace process
{
	typedef size_t id_t;

	namespace bitness
	{
		inline bool is_x64();
	}
}

namespace thread
{
	typedef size_t id_t;
}