/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   philosophers.h                                       ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/03 10:35:12 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/07 08:14:15 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
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

typedef struct s_shared	t_shared;
typedef struct s_thread	t_thread;
typedef struct s_config	t_config;

struct					s_shared
{
	pthread_mutex_t		**m_res;
	pthread_mutex_t		*m_io;
	pthread_mutex_t		*m_res_usage;
	t_config			*config;
	int					starved;
	long				t_start;
};

struct					s_thread
{
	t_shared			*shared;
	pthread_t			tid;
	pthread_mutex_t		*m_res[2];
	int					h_id;
	long				t_last_access;
	int					starved;
	int					n_access;
};

struct					s_config
{
	int					t_die;
	int					t_eat;
	int					t_sleep;
	int					n_threads;
	int					n_access;
};

long					timestamp(void);
long					delta_t(long t0, long t1);
void					isleep(long time);
void					_print(t_shared *shared, int id, char const *s);
int						ft_atoi(char const *nptr);

int						eat(t_thread *thread);
void					*routine(void *thrd);

t_shared				*shared_init(char **argv);
t_thread				*threads_init(t_shared *shared);
int						is_valid_int32(char const *nptr);
int						pos_int32_atoi(char const *nptr);

#endif
