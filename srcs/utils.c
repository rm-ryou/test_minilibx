#include "./miniRT.h"

bool	intersection(t_objects *object, t_ray *ray, t_point_intersection *intp)
{
	t_vector	point_inter;
	double		t = -1;
	if (object->type == SPHERE) {
		double		A;
		double		B;
		double		C;
		double		D;

		point_inter = sub(ray->start, object->position);
		A = dot(ray->direction, ray->direction);
		B = 2 * dot(ray->direction, point_inter);
		C = dot(point_inter, point_inter) - pow(object->radius, 2);
		D = pow(B, 2) - 4 * A * C;

		if (D == 0)
			t = -B / (2 * A);
		else if (D > 0) {
			double	t1 = (-B - sqrt(D)) / (2 * A);
			double	t2 = (-B + sqrt(D)) / (2 * A);
			t = (t1 > 0 && t2 > 0) ? (t1 < t2 ? t1: t2): (t1 > t2 ? t1: t2);
		}
		if (t > 0) {
			if (intp) {
				intp->distance = t;
				intp->position = add(ray->start, mult(ray->direction, t));
				intp->normal = normalize(sub(intp->position, object->position));
			}
			return (true);
		}
	} else if (object->type == PLANE) {
		double	dn_dot = dot(ray->direction, object->normal);

		if (dn_dot) {
			point_inter = sub(ray->start, object->position);
			t = -1 * dot(point_inter, object->normal) / dn_dot;

			if (t > 0) {
				if (intp) {
					intp->distance = t;
					intp->position = add(ray->start, mult(ray->direction, t));
					intp->normal = object->normal;
				}
				return (true);
			}	
		}
	}
	return (false);
}

bool	get_nearest_shape(t_scene *scene, t_ray *ray, t_objects **object, t_point_intersection *out_intp, int exit_once_found)
{
	size_t		i;
	t_objects	*nearest_shape = NULL;
	t_point_intersection	nearest_intp;
	nearest_intp.distance = INT_MAX;

	for (int i = 0; i < 6; i++) {
		t_point_intersection	intp;
		bool	is_intersection = intersection(&scene->objects[i], ray, &intp);
	
		if (is_intersection && intp.distance < nearest_intp.distance) {
			nearest_shape = &scene->objects[i];
			nearest_intp = intp;
			if (exit_once_found)
				break ;
		//	nearest_intp.distance = intp.distance;
		}
	}
	if (nearest_shape) {
		if (object)
			*object = nearest_shape;
		if (out_intp)
			*out_intp = nearest_intp;
		return (true);
	}
	return (false);
}

void	raytrace(t_scene *scene, t_ray *ray, t_color *color)
{
	int						is_object;
	t_objects				*object;
	t_point_intersection	point_of_object;

	is_object = get_nearest_shape(scene, ray, &object, &point_of_object, 0);
	if (is_object) {
		t_color	color_k = new_color(0, 0, 0);
		color_k.red += 0.01 * 0.01;// = add_color(color_k, scene->ambient_illuminance);
		color_k.green += 0.01 * 0.01;
		color_k.blue += 0.01 * 0.01;


		t_vector	light_dir;	// 入射ベクトル
		double		nl_dot;		// 法線ベクトル・入射ベクトル
		light_dir = normalize(sub(scene->light->light_position, point_of_object.position));
		nl_dot = dot(point_of_object.normal, light_dir);
		if (nl_dot <= 0)	nl_dot = 0;
		if (nl_dot >= 1)	nl_dot = 1;


		double		distance_light_dir = mag(sub(scene->light->light_position, point_of_object.position)) - (double)(1 / 32);
		t_vector	shadow_ray_vec = add(point_of_object.position, mult(light_dir, (double)1 / 32));
		t_ray		shadow_ray;
		shadow_ray.start = shadow_ray_vec;
		shadow_ray.direction = light_dir;
		if (get_nearest_shape(scene, &shadow_ray, &object, &point_of_object, 1)) {
			*color = color_k;
			return ;
		}



		color_k.red += 0.69 * 1.0 * nl_dot;
		color_k.green += 0.69 * 1.0 * nl_dot;
		color_k.blue += 0.69 * 1.0 * nl_dot;

		if (nl_dot) {
			t_vector	ref_dir;
			t_vector	rev_eye_dir;
			double		dot_ref_rev_eye;
			double		dot_ref_rev_eye_pow;

			ref_dir = normalize(sub(mult(point_of_object.normal, 2 * nl_dot), light_dir));
			rev_eye_dir = normalize(mult(ray->direction, -1));
			dot_ref_rev_eye = dot(ref_dir, rev_eye_dir);
			if (dot_ref_rev_eye <= 0)	dot_ref_rev_eye = 0;
			if (dot_ref_rev_eye >= 1)	dot_ref_rev_eye = 1;

			color_k.red += 0.3 * 1.0 * pow(dot_ref_rev_eye, 8);
			color_k.green += 0.3 * 1.0 * pow(dot_ref_rev_eye, 8);
			color_k.blue += 0.3 * 1.0 * pow(dot_ref_rev_eye, 8);
		}

		*color = color_k;
	}
}