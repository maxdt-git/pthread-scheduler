/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:22:59 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/06 14:46:21 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine_coder(void *args)
{
	t_coder		*coder;
	t_dongle	*dongle_right;
	t_dongle	*dongle_left;

	coder = (t_coder *)args;
	dongle_right = &coder->env->dongle[coder->right_dongle];
	dongle_left = &coder->env->dongle[coder->left_dongle];
	while (!is_stopped(coder->env)
		&& coder->number_compile_done
		< coder->env->config.number_of_compiles_required)
	{
		if (!take_both_dongles(coder))
			break ;
		update_compile_start(coder);
		log_print(coder->env, coder->id, " is compiling");
		usleep(coder->env->config.time_to_compile * 1000);
		release_dongles(dongle_right, dongle_left);
		log_print(coder->env, coder->id, " is debugging");
		usleep(coder->env->config.time_to_debug * 1000);
		log_print(coder->env, coder->id, " is refactoring");
		usleep(coder->env->config.time_to_refactor * 1000);
	}
	return (NULL);
}

int	helper_routine_burnout(t_env *env, int time_spent, int i)
{
	if (time_spent >= env->config.time_to_burnout)
	{
		stop_simu(env);
		pthread_mutex_lock(&env->mutex);
		printf("%d %d burned out\n", get_time_ms(env->start),
			env->coder[i].id);
		pthread_mutex_unlock(&env->mutex);
		return (0);
	}
	return (1);
}

void	*routine_burnout(void *args)
{
	struct timeval	now;
	t_env			*env;
	int				time_spent;
	int				i;

	env = (t_env *)args;
	while (!is_stopped(env))
	{
		if (all_done(env))
			break ;
		i = 0;
		while (i < env->config.number_of_coders)
		{
			pthread_mutex_lock(&env->coder[i].state_mutex);
			gettimeofday(&now, NULL);
			time_spent = diff_ms(&now, env->coder[i].last_compile_start);
			pthread_mutex_unlock(&env->coder[i].state_mutex);
			if (!helper_routine_burnout(env, time_spent, i))
				return (NULL);
			i++;
		}
		usleep(10);
	}
	return (NULL);
}
