/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 11:13:40 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/03 15:33:13 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	diff_ms(struct timeval *a, struct timeval *b)
{
	return ((a->tv_sec - b->tv_sec) * 1000 + (a->tv_usec - b->tv_usec) / 1000);
}

int	dongle_ready(t_dongle *dongle, struct timeval *now, int cooldown)
{
	if (!dongle->available)
		return (0);
	if (dongle->never_used)
		return (1);
	return (diff_ms(now, &dongle->released) >= cooldown);
}

struct timespec	deadline_ms(struct timeval *now, int remain)
{
	struct timespec	deadline;

	deadline.tv_sec = now->tv_sec + (remain / 1000);
	deadline.tv_nsec = (now->tv_usec + (remain % 1000) * 1000) * 1000;
	if (deadline.tv_nsec >= 1000000000)
	{
		deadline.tv_sec += 1;
		deadline.tv_nsec -= 1000000000;
	}
	return (deadline);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->never_used = 0;
	gettimeofday(&dongle->released, NULL);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_dongles(t_dongle *dongle1, t_dongle *dongle2)
{
	release_dongle(dongle1);
	release_dongle(dongle2);
}
