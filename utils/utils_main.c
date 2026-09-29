/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 10:57:35 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/05 14:27:35 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_env	*malloc_all(t_config parsed)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->config = parsed;
	env->coder = malloc(sizeof(t_coder) * parsed.number_of_coders);
	if (!env->coder)
		return (NULL);
	env->dongle = malloc(sizeof(t_dongle) * parsed.number_of_coders);
	if (!env->dongle)
		return (NULL);
	env->start = malloc(sizeof(struct timeval));
	if (!env->start)
		return (NULL);
	return (env);
}

void	helper_init(t_env *env, int i)
{
	env->coder[i].last_compile_start = malloc(sizeof(struct timeval));
	gettimeofday(env->coder[i].last_compile_start, NULL);
	env->coder[i].id = i + 1;
	env->coder[i].right_dongle = i;
	env->coder[i].left_dongle = (i + 1) % env->config.number_of_coders;
	env->coder[i].number_compile_done = 0;
	env->coder[i].env = env;
	env->dongle[i].id = i + 1;
	env->dongle[i].available = 1;
	env->dongle[i].never_used = 1;
	env->dongle[i].next_order = 0;
	env->dongle[i].queue_size = 0;
	pthread_mutex_init(&env->coder[i].state_mutex, NULL);
	pthread_mutex_init(&env->dongle[i].mutex, NULL);
	pthread_cond_init(&env->dongle[i].cond, NULL);
}

void	helper_thread(t_env *env)
{
	int	i;

	i = 0;
	pthread_create(&env->monitor, NULL, routine_burnout, env);
	while (i < env->config.number_of_coders)
	{
		pthread_create(&env->coder[i].thread, NULL, routine_coder,
			&env->coder[i]);
		i++;
	}
	i = 0;
	while (i < env->config.number_of_coders)
	{
		pthread_join(env->coder[i].thread, NULL);
		i++;
	}
	pthread_join(env->monitor, NULL);
}

t_env	*init(t_config parsed)
{
	int		i;
	t_env	*env;

	env = malloc_all(parsed);
	gettimeofday(env->start, NULL);
	env->stop = 0;
	pthread_mutex_init(&env->stop_mutex, NULL);
	pthread_mutex_init(&env->mutex, NULL);
	i = 0;
	while (i < parsed.number_of_coders)
	{
		helper_init(env, i);
		i++;
	}
	return (env);
}

void	clean_all(t_env *env)
{
	int	i;

	i = 0;
	free(env->start);
	pthread_mutex_destroy(&env->stop_mutex);
	pthread_mutex_destroy(&env->mutex);
	while (i < env->config.number_of_coders)
	{
		pthread_mutex_destroy(&env->dongle[i].mutex);
		pthread_cond_destroy(&env->dongle[i].cond);
		pthread_mutex_destroy(&env->coder[i].state_mutex);
		free(env->coder[i].last_compile_start);
		i++;
	}
	free(env->coder);
	free(env->dongle);
	free(env);
}
