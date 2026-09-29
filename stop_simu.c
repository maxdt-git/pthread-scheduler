/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 10:24:59 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/03 15:55:17 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	stop_simu(t_env *env)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&env->stop_mutex);
	env->stop = 1;
	while (i < env->config.number_of_coders)
	{
		pthread_cond_broadcast(&env->dongle[i].cond);
		i++;
	}
	pthread_mutex_unlock(&env->stop_mutex);
}

int	is_stopped(t_env *env)
{
	int	stopped;

	pthread_mutex_lock(&env->stop_mutex);
	stopped = env->stop;
	pthread_mutex_unlock(&env->stop_mutex);
	return (stopped);
}

int	all_done(t_env *env)
{
	int	i;
	int	done;

	i = 0;
	while (i < env->config.number_of_coders)
	{
		pthread_mutex_lock(&env->coder[i].state_mutex);
		done = (env->coder[i].number_compile_done
				>= env->config.number_of_compiles_required);
		pthread_mutex_unlock(&env->coder[i].state_mutex);
		if (!done)
			return (0);
		i++;
	}
	stop_simu(env);
	return (1);
}
