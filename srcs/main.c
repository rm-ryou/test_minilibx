#include "miniRT.h"

static void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	set_mlx(t_data *data)
{
	data->mlx = mlx_init();
	data->mlx_win = mlx_new_window(data->mlx, DISPLAY_W, DISPLAY_H, "miniRT");
	data->img = mlx_new_image(data->mlx, DISPLAY_W, DISPLAY_H);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel, &data->line_length, &data->endian);
}

void	init_scene(t_scene *scene)
{
	scene->objects = malloc(sizeof(t_objects) * 6);
	scene->light = malloc(sizeof(t_light));

	scene->objects[0].type = SPHERE;
	scene->objects[0].position = new_vector(3, 0, 25);
	scene->objects[0].radius = 1.0;

	scene->objects[1].type = SPHERE;
	scene->objects[1].position = new_vector(2, 0, 20);
	scene->objects[1].radius = 1.0;

	scene->objects[2].type = SPHERE;
	scene->objects[2].position = new_vector(1, 0, 15);
	scene->objects[2].radius = 1.0;

	scene->objects[3].type = SPHERE;
	scene->objects[3].position = new_vector(0, 0, 10);
	scene->objects[3].radius = 1.0;

	scene->objects[4].type = SPHERE;
	scene->objects[4].position = new_vector(-1, 0, 5);
	scene->objects[4].radius = 1.0;

	scene->objects[5].type = PLANE;
	scene->objects[5].position = new_vector(0, -1,  0);
	scene->objects[5].normal = new_vector(0, 1, 0);

	scene->light->light_position = new_vector(-5, 5, -5);
	scene->light->light_color = new_color(1.0, 1.0, 1.0);
}

int	main()
{
	t_data		data;
	t_vector	point_of_screen = new_vector(0, 0, 0);
	t_scene		scene;

	init_scene(&scene);
	set_mlx(&data);

	for (double y = 0; y < DISPLAY_H; y++) {
		point_of_screen.y = ((-2 * y) / ((double)DISPLAY_H - 1)) + 1.0;
		for (double x = 0; x < DISPLAY_W; x++) {
			point_of_screen.x = ((2 * x) / ((double)DISPLAY_W - 1)) - 1.0;
			t_color	color = new_color(0, 0, 1);
			t_ray	ray;
			ray.start = new_vector(0, 0, -5);
			ray.direction = sub(point_of_screen, ray.start);
			
			raytrace(&scene, &ray, &color);

			if (color.red <= 0)	color.red = 0;
			if (color.red >= 1)	color.red = 1;
			if (color.green <= 0)	color.green = 0;
			if (color.green >= 1)	color.green = 1;
			if (color.blue <= 0)	color.blue = 0;
			if (color.blue >= 1)	color.blue = 1;

			color.red *= 255;
			color.green *= 255;
			color.blue *= 255;

			my_mlx_pixel_put(&data, x, y, create_trgb(&color));
		}
	}
	mlx_put_image_to_window(data.mlx, data.mlx_win, data.img, 0, 0);
	mlx_loop(data.mlx);
	free(scene.objects);
	free(scene.light);
	return (0);
}