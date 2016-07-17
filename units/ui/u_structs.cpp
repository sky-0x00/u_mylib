#include "stdafx.h"
#include "u_structs.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ui::point_t::point_t(int by_width, int by_height): 
	by_width(by_width), by_height(by_height) {}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ui::rect_t::rect_t(const point_t &left_top, const point_t &right_bottom): 
	left_top(left_top), right_bottom(right_bottom) {}

ui::rect_t::rect_t(int left, int top, int right, int bottom):
	left(left), top(top), right(right), bottom(bottom) {}

int ui::rect_t::get_width() const
{
	return right - left;
}
int ui::rect_t::get_height() const
{
	return bottom - top;
}

void ui::rect_t::set_width(int new_width, bool is_fixed__right_bound)
{
	if (is_fixed__right_bound)
		left = right - new_width;
	else
		right = left + new_width;
}
void ui::rect_t::set_height(int new_height, bool is_fixed__bottom_bound)
{
	if (is_fixed__bottom_bound)
		top = bottom - new_height;
	else
		bottom = top + new_height;
}

ui::point_t ui::rect_t::get_center_point(bool is_absolut) const
{
	point_t result = { get_width() >> 1, get_height() >> 1 };
	if (is_absolut)
	{
		result.by_width += left;
		result.by_height += top;
	}
	return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------