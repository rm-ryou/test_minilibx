#ifndef MINIRT_H
# define MINIRT_H

# include <math.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>
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
	double	red;
	double	green;
	double	blue;
}	t_color;

typedef struct	s_ray
{
	t_vector	start;
	t_vector	direction;
}	t_ray;

typedef enum
{
	SPHERE,
	PLANE,
}	e_object_type;

typedef struct	s_sphere
{
	t_vector	center;
	double		radius;
}	t_sphere;

typedef struct	s_plane
{
	t_vector	normal;
	t_vector	position;
}	t_plane;

typedef	struct s_objects
{
	e_object_type	type;
	t_vector		position;
	t_vector		normal;
	double			radius;
}	t_objects;


typedef struct	s_material
{
	t_color	ambient_ref;
	t_color	diffuse_ref;
	t_color	specular_ref;
	double	shininess;
}	t_material;

typedef struct	s_light
{
	t_vector		light_position;
	t_color			light_color;
}	t_light;

typedef struct	s_point_intersection
{
	double		distance;
	t_vector	position;
	t_vector	normal;
}	t_point_intersection;

typedef struct	s_scene
{
	t_objects	*objects;
	t_light		*light;
	//t_color		ambient_illuminance;	// 環境光の強度
}	t_scene;

t_vector	new_vector(double x, double y, double z);
t_vector	add(t_vector a, t_vector b);
t_vector	sub(t_vector a, t_vector b);
t_vector	mult(t_vector a, double n);
double		dot(t_vector a, t_vector b);
double		mag(t_vector a);
double		mag_sqrt(t_vector a);
t_vector	normalize(t_vector a);

int			create_trgb(t_color *color);
t_color		new_color(double red, double green, double blue);
t_color		add_color(t_color a, t_color b);

void	raytrace(t_scene *scene, t_ray *ray, t_color *color);

#endif // !MINIRT_H