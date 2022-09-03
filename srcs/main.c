#include "miniRT.h"

static void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	create_trgb(t_color *color)
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

	t_vector	point_of_light = new_vector(-5, 5, -5);

	// branc -> simpleShading
	// 環境光反射係数
	double	ambient = 0.01;
	// 拡散反射係数
	double	diffusion_reflection = 0.69;
	// 鏡面反射係数
	double	specular_reflection = 0.3;
	// 光沢度
	double	gloss_level = 8;
	// 環境光の強度
	double	ambient_intensity = 0.1;
	// 光源の強度
	double	light_intensity = 1.0;

	set_mlx(&data);

	for (double y = 0; y < DISPLAY_H; y++) {
		point_of_screen.y = ((-2 * y) / ((double)DISPLAY_H - 1)) + 1.0;
		for (double x = 0; x < DISPLAY_W; x++) {
			point_of_screen.x = ((2 * x) / ((double)DISPLAY_W - 1)) - 1.0;

			t_vector	eye_dir = normalize(sub(point_of_screen, eye_vec));
			t_vector	tmp = sub(eye_vec, sphere_vec);

			double	A = mag_sqrt(eye_dir);
			double	B = 2 * dot(eye_dir, tmp);
			double	C = mag_sqrt(tmp) - pow(sphere_r, 2);

			double	D = pow(B, 2) - 4 * A * C;

			double	t = -1;
			if (D == 0)
				t = -B / (2 * A);
			else if (D > 0) {
				double	t1 = (-B - sqrt(D)) / (2 * A);
				double	t2 = (-B + sqrt(D)) / (2 * A);
				t = (t1 > 0 && t2 > 0) ? (t1 < t2 ? t1: t2): (t1 > t2 ? t1: t2);
			}
			if (t > 0) {
				double	ambient_radiance = ambient * ambient_intensity;
				// 放射輝度
				t_vector	point_of_intersection = add(eye_vec, mult(eye_dir, t));
				// 入射ベクトル
				t_vector	dir_of_light = normalize(sub(point_of_light, point_of_intersection));
				// 法線ベクトル
				t_vector	normal_vec = normalize(sub(point_of_intersection, sphere_vec));

				double	dot_of_light_normal = dot(normal_vec, dir_of_light);
				if (dot_of_light_normal <= 0)	dot_of_light_normal = 0;
				if (dot_of_light_normal >= 1)	dot_of_light_normal = 1;

				double	diffusion_radiance = diffusion_reflection * light_intensity * dot_of_light_normal;

				double	specular_radiance = 0;
				if (dot_of_light_normal > 0) {
					t_vector	reflect_vec = sub(mult(normal_vec, 2 * dot_of_light_normal), dir_of_light);
					t_vector	rev_view_vec = normalize(mult(eye_dir, -1));
					/*rev_view_vec.x = -eye_dir.x;
					rev_view_vec.y = -eye_dir.y;
					rev_view_vec.z = -eye_dir.z;*/

					double	dot_reflect_rev_view = dot(reflect_vec, rev_view_vec);
					if (dot_reflect_rev_view <= 0)	dot_reflect_rev_view = 0;
					if (dot_reflect_rev_view >= 1)	dot_reflect_rev_view = 1;
					specular_radiance = specular_reflection * light_intensity * pow(dot_reflect_rev_view, gloss_level);
				}
				double	reflect_radiance = ambient_radiance + diffusion_radiance + specular_radiance;
				if (reflect_radiance < 0)	reflect_radiance = 0;
				if (reflect_radiance > 1)	reflect_radiance = 1;

				t_color		color;
				color.red = 255 * reflect_radiance;
				color.green = 0;//255 * reflect_radiance;
				color.blue = 0;//255 * reflect_radiance;

				my_mlx_pixel_put(&data, x, y, create_trgb(&color));
			} else
				my_mlx_pixel_put(&data, x, y, 0x0000FF);
			


/*			if (D >= 0)
				my_mlx_pixel_put(&data, x, y, 0xFF0000);
			else
				my_mlx_pixel_put(&data, x, y, 0x0000FF);*/
		}
	}
	mlx_put_image_to_window(data.mlx, data.mlx_win, data.img, 0, 0);
	mlx_loop(data.mlx);
	return (0);
}