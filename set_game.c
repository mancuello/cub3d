/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuello <mcuello@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:51:59 by mcuello           #+#    #+#             */
/*   Updated: 2026/03/25 16:09:22 by mcuello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	rgb_to_pixel(char *rgb, uint32_t *color)
{
	char	**arr;
	int		r;
	int		g;
	int		b;

	arr = ft_split(rgb, ',');
	if (!arr)
		return (-1);
	if (!arr[0] || !arr[1] || !arr[2] || arr[3])
		return (free(arr), -1);
	r = ft_atoi(arr[0]);
	g = ft_atoi(arr[1]);
	b = ft_atoi(arr[2]);
	free_map(arr);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	*color = (r << 24 | g << 16 | b << 8 | 0xFF);
	return (0);
}

int	set_game(t_fd *fd, t_game *game)
{
	game->player.x = fd->map->pos_x + 0.5;
	game->player.y = fd->map->pos_y + 0.5;
	game->player.angle = 90.05;
	game->map = fd->map;
	if (rgb_to_pixel(fd->ceiling_color, &game->ceiling) == -1)
		return (ft_error("Error en el color rgb"), -1);
	if (rgb_to_pixel(fd->floor_color, &game->floor) == -1)
		return (ft_error("Error en el color rgb"), -1);
	return (0);
}
