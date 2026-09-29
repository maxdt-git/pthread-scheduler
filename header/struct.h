/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 13:41:08 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/05 12:10:51 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "include.h"

typedef struct s_config
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	int		scheduler;
}	t_config;

typedef struct s_request
{
	int	coder_id;
	int	order;
	int	deadline;
}	t_request;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	struct timeval	released;
	t_request		queue[2];
	int				queue_size;
	int				id;
	int				never_used;
	int				available;
	int				next_order;
}	t_dongle;

typedef struct s_coder		t_coder;

typedef struct s_env
{
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	mutex;
	pthread_t		monitor;
	struct timeval	*start;
	t_config		config;
	t_coder			*coder;
	t_dongle		*dongle;
	int				stop;
}	t_env;

typedef struct s_coder
{
	t_env			*env;
	pthread_t		thread;
	pthread_mutex_t	state_mutex;
	struct timeval	*last_compile_start;
	int				id;
	int				right_dongle;
	int				left_dongle;
	int				number_compile_done;
}	t_coder;

#endif
