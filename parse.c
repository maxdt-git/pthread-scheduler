/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 14:02:47 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/04 16:45:55 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_int(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	parse_args(char **av, t_config *config)
{
	if (!check_int(av[1]) || !check_int(av[2]) || !check_int(av[3])
		|| !check_int(av[4]) || !check_int(av[5]) || !check_int(av[6])
		|| !check_int(av[7]))
	{
		printf("Wrong args, should be 7 int and the scheduler\n");
		return (0);
	}
	config->number_of_coders = atoi(av[1]);
	config->time_to_burnout = atoi(av[2]);
	config->time_to_compile = atoi(av[3]);
	config->time_to_debug = atoi(av[4]);
	config->time_to_refactor = atoi(av[5]);
	config->number_of_compiles_required = atoi(av[6]);
	config->dongle_cooldown = atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		config->scheduler = 0;
	if (strcmp(av[8], "edf") == 0)
		config->scheduler = 1;
	if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "edf") != 0)
	{
		printf("Wrong scheduler\n");
		return (0);
	}
	return (1);
}
