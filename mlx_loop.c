#include "cub3d.h"

static void ft_hook(t_game *data)
{
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE) || mlx_is_key_down(data->mlx, MLX_KEY_Q))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->player.angle += 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->player.angle -= 1;

	double rad = data->player.angle * ANGLE_TO_RADIAN;
	double speed = 0.1; //ANOTHER data->player STAT
	char **map = data->map->map;

	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		if (map[(int)(data->player.y - sin(rad) * speed)][(int)(data->player.x)] != '1')
			data->player.y -= sin(rad) * speed;
		if (map[(int)(data->player.y)][(int)(data->player.x + cos(rad) * speed)] != '1')
			data->player.x += cos(rad) * speed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		if (map[(int)(data->player.y + sin(rad) * speed)][(int)(data->player.x)] != '1')
			data->player.y += sin(rad) * speed;
		if (map[(int)(data->player.y)][(int)(data->player.x - cos(rad) * speed)] != '1')
			data->player.x -= cos(rad) * speed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		if (map[(int)(data->player.y - cos(rad) * speed)][(int)(data->player.x)] != '1')
			data->player.y -= cos(rad) * speed;
		if (map[(int)(data->player.y)][(int)(data->player.x - sin(rad) * speed)] != '1')
			data->player.x -= sin(rad) * speed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		if (map[(int)(data->player.y + cos(rad) * speed)][(int)(data->player.x)] != '1')
			data->player.y += cos(rad) * speed;
		if (map[(int)(data->player.y)][(int)(data->player.x + sin(rad) * speed)] != '1')
			data->player.x += sin(rad) * speed;
	}
}

void loop_func(void *param)
{
	t_game *game = (t_game *)param;
	ft_hook(game);

	mlx_texture_t *tex;

	double ray_angle = fmod(game->player.angle + FOV / 2 , 360);
	//double rays[WIDTH]; para luego si eso saber que muro, tal vez necestiemos esto
	int i = 0;
	while (i < RES_WIDTH)
	{
		//printf("Angle: %d  Radians: %f  Distance: ", angle, data->player.angle);
		if(ray_angle < 0)
			ray_angle += 360;
		//rays[i] = cast_ray(ray_angle) * cos((ray_angle - data->player.angle) * ANGLE_TO_RADIAN);
		t_ray ray = cast_ray(ray_angle, game);

		if (ray.face == 'N') tex = game->text_n;
		else if (ray.face == 'S') tex = game->text_s;
		else if (ray.face == 'E') tex = game->text_e;
		else tex = game->text_w;
		int tex_x = (int)(ray.wall_percent * tex->width);
		
	//prnitign pixels for each ray
		int wall_size = (int)(RES_HEIGHT / (ray.dist * 3));
		int real_wall_size = wall_size;          // keep real value
		if (wall_size > RES_HEIGHT) wall_size = RES_HEIGHT;
		int top = (RES_HEIGHT - real_wall_size) / 2; // use real size for centering
		int draw_start = top < 0 ? 0 : top;
		int draw_end = (top + real_wall_size) > RES_HEIGHT ? RES_HEIGHT : top + real_wall_size;
		double tex_pos_start = top < 0 ? (double)(-top) / real_wall_size : 0.0;

		int y = 0;
		while (y < draw_start)
			mlx_put_pixel(game->img, i, y++, game->ceiling);
		while (y < draw_end)
		{
			double tex_pos = tex_pos_start + (double)(y - draw_start) / real_wall_size;
			int tex_y = (int)(tex_pos * tex->height);
			int idx = (tex_y * tex->width + tex_x) * 4;
			uint32_t color = (tex->pixels[idx] << 24 |
				              tex->pixels[idx + 1] << 16 |
				              tex->pixels[idx + 2] << 8 |
				              tex->pixels[idx + 3]);
			mlx_put_pixel(game->img, i, y++, color);
		}
		while (y < RES_HEIGHT)
			mlx_put_pixel(game->img, i, y++, game->floor);
		
		ray_angle -= (double)FOV/RES_WIDTH; //tiene que castearse asi, si no, se queda en int
		i++;
	}
}

/* void loop_func(void *param)
{
	t_game *game = (t_game *)param;
	ft_hook(game);
	printf("pos x:%f pos y:%f angle: %f\n", game->player.X, game->player.Y, game->player.angle);
	mlx_texture_t *tex;

	double ray_angle = fmod(game->player.angle + FOV / 2, 360);
	if (ray_angle < 0)
		ray_angle += 360;
	int i = 0;
	while (i < RES_WIDTH)
	{
		if (ray_angle < 0)
			ray_angle += 360;
		if (ray_angle >= 360)
			ray_angle -= 360;
		t_ray ray = cast_ray(ray_angle, game);

		if (ray.face == 'N') tex = game->text_n;
		else if (ray.face == 'S') tex = game->text_s;
		else if (ray.face == 'E') tex = game->text_e;
		else tex = game->text_w;
		int tex_x = (int)(ray.wall_percent * tex->width);

		int wall_size = (int)(RES_HEIGHT / (ray.dist * 3));
		int real_wall_size = wall_size;
		if (wall_size > RES_HEIGHT) wall_size = RES_HEIGHT;
		int top = (RES_HEIGHT - real_wall_size) / 2;
		int draw_start = top < 0 ? 0 : top;
		int draw_end = (top + real_wall_size) > RES_HEIGHT ? RES_HEIGHT : top + real_wall_size;
		double tex_pos_start = top < 0 ? (double)(-top) / real_wall_size : 0.0;

		int y = 0;
		while (y < draw_start)
			mlx_put_pixel(game->img, i, y++, game->ceiling);
		while (y < draw_end)
		{
			double tex_pos = tex_pos_start + (double)(y - draw_start) / real_wall_size;
			int tex_y = (int)(tex_pos * tex->height);
			int idx = (tex_y * tex->width + tex_x) * 4;
			uint32_t color = (tex->pixels[idx] << 24 |
				              tex->pixels[idx + 1] << 16 |
				              tex->pixels[idx + 2] << 8 |
				              tex->pixels[idx + 3]);
			mlx_put_pixel(game->img, i, y++, color);
		}
		while (y < RES_HEIGHT)
			mlx_put_pixel(game->img, i, y++, game->floor);

		ray_angle -= (double)FOV / RES_WIDTH;
		i++;
	}
} */
