#include "miniRT.h"

static void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	test_create_trgb(t_color *color)
{
	return (color->red << 16 | color->green << 8 | color->blue);
}

void	set_mlx(t_data *data)
{
	data->mlx = mlx_init();
	data->mlx_win = mlx_new_window(data->mlx, DISPLAY_W, DISPLAY_H, "miniRT");
	data->img = mlx_new_image(data->mlx, DISPLAY_W, DISPLAY_H);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel, &data->line_length, &data->endian);
}

int	main()
{
	t_data	data;

	t_vector	eye_vec = new_vector(0, 0, -5);
	t_vector	sphere_vec = new_vector(0, 0, 5);
	t_vector	point_of_screen = new_vector(0, 0, 0);
	double		sphere_r = 1.0;

	set_mlx(&data);

	for (double y = 0; y < DISPLAY_H; y++) {
		point_of_screen.y = ((-2 * y) / (DISPLAY_H - 1)) + 1.0;
		for (double x = 0; x < DISPLAY_W; x++) {
			point_of_screen.x = ((2 * x) / (DISPLAY_W - 1)) - 1.0;

			t_vector	eye_dir = normalize(sub(point_of_screen, eye_vec));
			t_vector	tmp = sub(eye_vec, sphere_vec);

			double	A = mag_sqrt(eye_dir);
			double	B = 2 * dot(eye_dir, tmp);
			double	C = mag_sqrt(tmp) - pow(sphere_r, 2);

			double	D = pow(B, 2) - 4 * A * C;
			if (D >= 0)
				my_mlx_pixel_put(&data, x, y, 0xFF0000);
			else
				my_mlx_pixel_put(&data, x, y, 0x0000FF);
		}
	}
	mlx_put_image_to_window(data.mlx, data.mlx_win, data.img, 0, 0);
	mlx_loop(data.mlx);
	return (0);
}