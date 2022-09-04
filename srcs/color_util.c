#include "miniRT.h"

int	create_trgb(t_color *color)
{
	return ((int)color->red << 16 | (int)color->green << 8 | (int)color->blue);
}

t_color	new_color(double red, double green, double blue)
{
	t_color	new;

	new.red = red;
	new.green = green;
	new.blue = blue;
	return (new);
}

t_color	add_color(t_color a, t_color b)
{
	t_color	new;

	new.red = a.red + b.red;
	new.green = a.green + b.green;
	new.blue = a.blue + b.blue;
	return (new);
}