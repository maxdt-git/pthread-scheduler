/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 08:58:26 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/06 09:58:42 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_time_ms(struct timeval *start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start->tv_sec) * 1000
		+ (now.tv_usec - start->tv_usec) / 1000);
}

void	log_print(t_env *env, int coder_id, char *state)
{
	long	time;

	pthread_mutex_lock(&env->mutex);
	if (is_stopped(env))
	{
		pthread_mutex_unlock(&env->mutex);
		return ;
	}
	time = get_time_ms(env->start);
	printf("%ld %d%s\n", time, coder_id, state);
	if (strcmp(state, " is compiling") == 0)
		env->coder[coder_id - 1].number_compile_done++;
	pthread_mutex_unlock(&env->mutex);
}

void	update_compile_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_mutex);
	gettimeofday(coder->last_compile_start, NULL);
	pthread_mutex_unlock(&coder->state_mutex);
}
