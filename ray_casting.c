#include "cub3d.h"

static double pitagoras(double collision_x, double collision_y, t_game *data)
{
	double hypotenuse;
	
	hypotenuse = sqrt(
	(collision_x - data->player.x) * (collision_x - data->player.x) +
	(collision_y - data->player.y) * (collision_y - data->player.y));
	
	return (hypotenuse);
}

//horizontal collision, ray goes in X axis
static double h_collision(double angle, t_game *data, t_ray *h_ray)//data->player and map
{
	double constant_increment_x = TILE_SIZE * ((angle >= PIH && angle <= PI3H) ? -1 : 1); //
	double constant_increment_y = TILE_SIZE * fabs(tan(angle)) * ((angle >= 0 && angle <= PI) ? -1 : 1);
	
	h_ray->collision_x = ((int)(data->player.x/TILE_SIZE) + (angle < PIH || angle > PI3H)) * TILE_SIZE;
	h_ray->collision_y = data->player.y - (h_ray->collision_x - data->player.x) * tan(angle);
	
	int i = -1;
	while (++i < MAX_RAY_ITERATION)
	{
		int matrix_x = (int)(h_ray->collision_x / TILE_SIZE);
		int matrix_y = (int)(h_ray->collision_y / TILE_SIZE);
		
		if (matrix_x >= 0 && matrix_x < data->map->width
			&& matrix_y >= 0 && matrix_y < data->map->height
			&& (data->map->map[matrix_y][matrix_x - (constant_increment_x < 0)] == '1' || data->map->map[matrix_y][matrix_x - (constant_increment_x < 0)] == ' '))
				return (pitagoras(h_ray->collision_x, h_ray->collision_y, data));
		h_ray->collision_x += constant_increment_x;
		h_ray->collision_y += constant_increment_y;
	}
	return (pitagoras(h_ray->collision_x, h_ray->collision_y, data));
}

//vertical collision, ray goes in Y axis
static double v_collision(double angle, t_game *data, t_ray *v_ray)//data->player and map
{
	double constant_increment_y = TILE_SIZE * ((angle >= 0 && angle <= PI) ? -1 : 1);
	double constant_increment_x = TILE_SIZE / fabs(tan(angle)) * ((angle >= PIH && angle <= PI3H) ? -1 : 1);
	
	v_ray->collision_y = ((int)(data->player.y/TILE_SIZE) + (angle > PI && angle < (2 * PI))) * TILE_SIZE;
	v_ray->collision_x = data->player.x - (v_ray->collision_y - data->player.y) / tan(angle);
	
	int i = -1;
	while (++i < MAX_RAY_ITERATION)
	{
		int matrix_x = (int)(v_ray->collision_x / TILE_SIZE);
		int matrix_y = (int)(v_ray->collision_y / TILE_SIZE);
		if (matrix_x >= 0 && matrix_x < data->map->width
			&& matrix_y >= 0 && matrix_y < data->map->height
			&& (data->map->map[matrix_y - (constant_increment_y < 0)][matrix_x] == '1' || data->map->map[matrix_y - (constant_increment_y < 0)][matrix_x] == ' '))
				return (pitagoras(v_ray->collision_x, v_ray->collision_y, data));
		v_ray->collision_x += constant_increment_x;
		v_ray->collision_y += constant_increment_y;
	}
	return (pitagoras(v_ray->collision_x, v_ray->collision_y, data));
}

t_ray cast_ray(double angle, t_game *data) //data->player and map
{
	double rad_angle = angle * ANGLE_TO_RADIAN;
	t_ray h_ray;
	t_ray v_ray;
	t_ray final_ray;
	
	h_ray.dist = h_collision(rad_angle, data, &h_ray);
	v_ray.dist = v_collision(rad_angle, data, &v_ray);
	if (h_ray.dist < v_ray.dist)
	{
		final_ray.dist = h_ray.dist * cos((angle - data->player.angle) * ANGLE_TO_RADIAN);
		final_ray.wall_percent = h_ray.collision_y - (int)h_ray.collision_y; //el rayo al ser a la derecha, veremos el muro con y variante
		if (rad_angle > PIH && rad_angle < PI3H)
			final_ray.face = 'W';
		else
			final_ray.face = 'E';
	}
	else
	{
		final_ray.dist = v_ray.dist * cos((angle - data->player.angle) * ANGLE_TO_RADIAN);
		final_ray.wall_percent = v_ray.collision_x - (int)v_ray.collision_x; //el rayo al ser en hroizontal, veremos el muro con y variante
		if (rad_angle > 0 && rad_angle < PI)
			final_ray.face = 'N';
		else
			final_ray.face = 'S';
	}
	
	if (final_ray.face == 'S' || final_ray.face == 'W') //para que los muros no se impirmanal reves
    	final_ray.wall_percent = 1.0 - final_ray.wall_percent;

	//return(fmin(h_collision(rad_angle, data), v_collision(rad_angle, data)));
	//temp
	return (final_ray);
}