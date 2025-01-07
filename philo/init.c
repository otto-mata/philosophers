/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   init.c                                               ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/03 10:39:12 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/07 08:30:37 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	mutexes_init(t_shared *shared)
{
	int	i;

	i = -1;
	shared->m_res = calloc(shared->config->n_threads,
			sizeof(pthread_mutex_t *));
	if (!shared->m_res)
		return (1);
	while (++i < shared->config->n_threads)
	{
		shared->m_res[i] = calloc(1, sizeof(pthread_mutex_t));
		pthread_mutex_init(shared->m_res[i], 0);
	}
	shared->m_io = calloc(1, sizeof(pthread_mutex_t));
	if (!shared->m_io)
		return (1);
	shared->m_res_usage = calloc(1, sizeof(pthread_mutex_t));
	if (!shared->m_io)
		return (1);
	pthread_mutex_init(shared->m_io, 0);
	pthread_mutex_init(shared->m_res_usage, 0);
	return (0);
}

static int	config_init(t_config *config, char **argv)
{
	config->n_access = -1;
	config->n_threads = pos_int32_atoi(argv[1]);
	config->t_die = pos_int32_atoi(argv[2]);
	config->t_eat = pos_int32_atoi(argv[3]);
	config->t_sleep = pos_int32_atoi(argv[4]);
	if (argv[5])
		config->n_access = pos_int32_atoi(argv[5]);
	if (!config->n_access || config->n_threads < 2 || config->t_die < 0
		|| config->t_eat < 0 || config->t_sleep < 0)
		return (1);
	return (0);
}

t_thread	*threads_init(t_shared *shared)
{
	t_thread	*threads;
	int			i;

	threads = calloc(shared->config->n_threads, sizeof(t_thread));
	if (!threads)
		return (0);
	i = -1;
	while (++i < shared->config->n_threads)
	{
		threads[i].h_id = i;
		threads[i].shared = shared;
		threads[i].m_res[0] = shared->m_res[i];
		threads[i].m_res[1] = shared->m_res[(i + 1)
			% shared->config->n_threads];
	}
	return (threads);
}

t_shared	*shared_init(char **argv)
{
	t_shared	*shared;

	shared = calloc(1, sizeof(t_shared));
	if (!shared)
		return (0);
	shared->config = calloc(1, sizeof(t_config));
	if (!shared->config)
		return (0);
	if (config_init(shared->config, argv))
		return (0);
	if (mutexes_init(shared))
		return (0);
	return (shared);
}
