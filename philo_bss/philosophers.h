/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   philosophers.h                                       ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/03 10:35:12 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/24 16:56:31 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# define __USE_XOPEN_EXTENDED
# include <unistd.h>

typedef struct s_shared		t_shared;
typedef struct s_thread		t_thread;
typedef struct s_config		t_config;
typedef struct s_supervisor	t_supervisor;

enum						e_pseudo_defines
{
	MAX_THREADS = 200,
	RESERVED_SPACE_SIZE = sizeof(pthread_mutex_t) * MAX_THREADS,
};

struct						s_config
{
	int						t_die;
	int						t_eat;
	int						t_sleep;
	int						n_threads;
	int						n_access;
};

struct						s_shared
{
	pthread_mutex_t			m_io;
	pthread_mutex_t			m_0xdeadbeef;
	t_config				cfg;
	int						starved;
	long					t_start;
	int						init;
	pthread_mutex_t			m_res[MAX_THREADS];
};

struct						s_thread
{
	pthread_t				tid;
	pthread_mutex_t			*m_res[2];
	pthread_mutex_t			m_res_usage;
	int						h_id;
	long					t_last_access;
	int						starved;
	int						n_access;
};

struct						s_supervisor
{
	t_thread				threads[MAX_THREADS];
	int						n_threads;
	int						init;
};


#endif
