/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 15:41:58 by mde-nas           #+#    #+#             */
/*   Updated: 2026/07/06 09:52:18 by mde-nas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNC_H
# define FUNC_H

# include "codexion.h"

int				parse_args(char **av, t_config *config);
void			*routine_coder(void *args);
t_env			*init(t_config parsed);
t_env			*malloc_all(t_config parsed);
int				get_time_ms(struct timeval *start);
void			log_print(t_env *env, int coder_id, char *state);
t_env			*malloc_all(t_config parsed);
t_env			*init(t_config parsed);
void			clean_all(t_env *env);
int				diff_ms(struct timeval *a, struct timeval *b);
int				dongle_ready(t_dongle *dongle, struct timeval *now,
					int cooldown);
struct timespec	deadline_ms(struct timeval *now, int remain);
int				dongle_try_acquire(t_coder *coder, t_dongle *dongle,
					struct timeval *now);
int				take_dongle(t_coder *coder, t_dongle *dongle);
int				take_both_dongles(t_coder *coder);
void			release_dongle(t_dongle *dongle);
void			release_dongles(t_dongle *dongle1, t_dongle *dongle2);
void			update_compile_start(t_coder *coder);
void			stop_simu(t_env *env);
int				is_stopped(t_env *env);
int				all_done(t_env *env);
void			*routine_burnout(void *args);
void			helper_thread(t_env *env);
int				request_is_before(t_env *env, t_request a, t_request b);
void			heap_push(t_env *env, t_dongle *dongle, t_request req);
t_request		heap_pop(t_dongle *dongle);
t_request		heap_top(t_dongle *dongle);

#endif
