/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 11:13:38 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/06 14:57:29 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_is_before(t_env *env, t_request a, t_request b)
{
	if (env->config.scheduler == 0)
	{
		if (a.order < b.order)
			return (1);
		return (0);
	}
	if (env->config.scheduler == 1)
	{
		if (a.deadline < b.deadline)
			return (1);
		if (a.deadline == b.deadline)
		{
			if (a.order < b.order)
				return (1);
			return (0);
		}
		return (0);
	}
	return (0);
}

void	heap_push(t_env *env, t_dongle *dongle, t_request req)
{
	t_request	swap;

	if (dongle->queue_size == 0)
	{
		dongle->queue[0] = req;
		dongle->queue_size = 1;
	}
	else
	{
		dongle->queue[1] = req;
		dongle->queue_size = 2;
	}
	if (dongle->queue_size == 2 && request_is_before(env, dongle->queue[0],
			dongle->queue[1]) == 0)
	{
		swap = dongle->queue[0];
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = swap;
	}
}

t_request	heap_pop(t_dongle *dongle)
{
	t_request	pop;

	pop = dongle->queue[0];
	if (dongle->queue_size == 1)
		dongle->queue_size = 0;
	if (dongle->queue_size == 2)
	{
		dongle->queue[0] = dongle->queue[1];
		dongle->queue_size = 1;
	}
	return (pop);
}

t_request	heap_top(t_dongle *dongle)
{
	return (dongle->queue[0]);
}
