/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extension_rules.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuello <mcuello@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:05:27 by mcuello           #+#    #+#             */
/*   Updated: 2026/03/29 21:14:18 by mcuello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_extension(char *str)
{
	char	*sub;

	if (ft_strlen(str) <= 4)
		return (-1);
	if (str[ft_strlen(str) - 5] == '/')
		return (-1);
	sub = ft_substr(str, ft_strlen(str) - 4, 4);
	if (!sub)
		return (-1);
	if (ft_strcmp(sub, ".cub"))
	{
		free(sub);
		return (-1);
	}
	free(sub);
	return (0);
}
