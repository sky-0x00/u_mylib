#pragma once

namespace ui
{
	class point_t
	{
	public:
		int by_width, by_height;
		point_t(int by_width = 0, int by_height = 0);
	};

	class rect_t
	{
	public:
		union
		{
			point_t left_top;
			struct
			{
				int left, top;
			};
		};
		union
		{
			point_t right_bottom;
			struct
			{
				int right, bottom;
			};
		};

		int get_width() const;
		int get_height() const;

		void set_width(int new_width, bool is_fixed__right_bound = false);
		void set_height(int new_height, bool is_fixed__bottom_bound = false);

		point_t get_center_point(bool is_absolut = true) const;

		rect_t(const point_t &left_top, const point_t &right_bottom);
		rect_t(int left = 0, int top = 0, int right = 0, int bottom = 0);

	private:
	};
}