#ifndef MINIRT_H
# define MINIRT_H

# include <math.h>
# include <stdio.h>
# include "../mlx_linux/mlx.h"

# define DISPLAY_W 800
# define DISPLAY_H 800

typedef struct	s_data
{
	void	*mlx;
	void	*mlx_win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct	s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

typedef struct	s_color
{
	int	red;
	int	green;
	int	blue;
}	t_color;


t_vector	new_vector(double x, double y, double z);
t_vector	add(t_vector a, t_vector b);
t_vector	sub(t_vector a, t_vector b);
t_vector	mult(t_vector a, double n);
double		dot(t_vector a, t_vector b);
double		mag(t_vector a);
double		mag_sqrt(t_vector a);
t_vector	normalize(t_vector a);

#endif // !MINIRT_H