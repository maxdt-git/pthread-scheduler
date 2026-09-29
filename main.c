/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 15:40:31 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/05 12:34:40 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_config	parsed;
	t_env		*env;

	if (ac != 9)
	{
		printf("wrong nbrs of args");
		return (0);
	}
	if (!parse_args(av, &parsed))
		return (0);
	env = init(parsed);
	helper_thread(env);
	clean_all(env);
	return (0);
}
