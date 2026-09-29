/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 13:20:46 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/06 14:55:41 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->id % 2 == 1)
	{
		first = &coder->env->dongle[coder->right_dongle];
		second = &coder->env->dongle[coder->left_dongle];
	}
	else
	{
		first = &coder->env->dongle[coder->left_dongle];
		second = &coder->env->dongle[coder->right_dongle];
	}
	if (!take_dongle(coder, first))
		return (0);
	log_print(coder->env, coder->id, " is taking a dongle");
	if (!take_dongle(coder, second))
	{
		release_dongle(first);
		return (0);
	}
	log_print(coder->env, coder->id, " is taking a dongle");
	return (1);
}

void	helper_take_dongle(t_env *env, struct timeval now, t_dongle *dongle)
{
	struct timespec	deadline;
	int				remain;

	remain = env->config.dongle_cooldown - diff_ms(&now,
			&dongle->released);
	deadline = deadline_ms(&now, remain);
	pthread_cond_timedwait(&dongle->cond,
		&dongle->mutex, &deadline);
}

void	start_take_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	req;
	t_env		*env;

	env = coder->env;
	pthread_mutex_lock(&dongle->mutex);
	req.coder_id = coder->id;
	req.order = dongle->next_order++;
	req.deadline = diff_ms(coder->last_compile_start, coder->env->start)
	+ env->config.time_to_burnout;
	heap_push(env, dongle, req);
	pthread_cond_broadcast(&dongle->cond);
}

int	dongle_try_take(t_coder *coder, t_dongle *dongle, struct timeval *now)
{
	if (heap_top(dongle).coder_id != coder->id)
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
		return (1);
	}
	if (dongle_ready(dongle, now, coder->env->config.dongle_cooldown))
	{
		dongle->available = 0;
		heap_pop(dongle);
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	if (dongle->available)
		helper_take_dongle(coder->env, *now, dongle);
	else
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	return (0);
}

int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timeval	now;
	int				res;

	start_take_dongle(coder, dongle);
	while (1)
	{
		if (is_stopped(coder->env))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		gettimeofday(&now, NULL);
		usleep(500000);
		res = dongle_try_take(coder, dongle, &now);
		if (res != 0)
			return (res);
	}
}
