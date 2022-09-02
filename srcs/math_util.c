#include "miniRT.h"

t_vector	new_vector(double x, double y, double z)
{
	t_vector	new;

	new.x = x;
	new.y = y;
	new.z = z;
	return (new);
}

t_vector	add(t_vector a, t_vector b)
{
	t_vector	new;

	new.x = a.x + b.x;
	new.y = a.y + b.y;
	new.z = a.z + b.z;
	return (new);
}

t_vector	sub(t_vector a, t_vector b)
{
	t_vector	new;

	new.x = a.x - b.x;
	new.y = a.y - b.y;
	new.z = a.z - b.z;
	return (new);
}

t_vector	mult(t_vector a, int n)
{
	t_vector	new;

	new.x = a.x * n;
	new.y = a.y * n;
	new.z = a.z * n;
	return (new);
}

double	dot(t_vector a, t_vector b)
{
	double	res;

	res = 0;
	res += a.x * b.x;
	res += a.y * b.y;
	res += a.z * b.z;
	return (res);
}

double	mag(t_vector a)
{
	double	res;

	res = 0;
	res += pow(a.x, 2);
	res += pow(a.y, 2);
	res += pow(a.z, 2);
	return (sqrt(res));
}

double	mag_sqrt(t_vector a)
{
	double	res;

	res = 0;
	res += pow(a.x, 2);
	res += pow(a.y, 2);
	res += pow(a.z, 2);
	return (res);
}

t_vector	normalize(t_vector a)
{
	double		norm;
	t_vector	new;

	norm = mag(a);
	new.x = a.x / norm;
	new.y = a.y / norm;
	new.z = a.z / norm;
	return (new);
}
