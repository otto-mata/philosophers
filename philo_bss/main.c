/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.c                                               ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/24 14:55:57 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/31 12:20:35 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

long	delta_t(long t0, long t1)
{
	return (t1 - t0);
}

void	isleep(long time)
{
	long	i;

	i = timestamp();
	while (1)
	{
		if (delta_t(i, timestamp()) >= time)
			break ;
		usleep(50);
	}
}

int	ft_atoi(char const *nptr)
{
	int	sign;
	int	res;

	sign = 1;
	res = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		sign -= (*nptr == '-') * 2;
		nptr++;
	}
	while (*nptr && (*nptr >= '0' && *nptr <= '9'))
	{
		res *= 10;
		res += *nptr - '0';
		nptr++;
	}
	return (res * sign);
}

t_shared	*get_shared(void)
{
	static t_shared	shared = {0};

	if (!shared.init)
		shared.init = 1;
	return (&shared);
}

t_supervisor	*get_supervisor(void)
{
	static t_supervisor	spv = {0};

	if (!spv.init)
		spv.init = 1;
	return (&spv);
}
void	threads_init(void)
{
	t_thread	*threads;
	int			i;

	threads = get_supervisor()->threads;
	i = -1;
	while (++i < get_supervisor()->n_threads)
	{
		threads[i].h_id = i + 1;
		threads[i].m_res[0] = &get_shared()->m_res[i];
		threads[i].m_res[1] = &get_shared()->m_res[(i + 1)
			% get_supervisor()->n_threads];
		pthread_mutex_init(&threads[i].m_res_usage, 0);
	}
}

int	init_shared(int argc, char **argv)
{
	int	i;

	get_shared()->cfg.n_threads = ft_atoi(argv[1]);
	if (get_shared()->cfg.n_threads < 1 || get_shared()->cfg.n_threads > 200)
		return (0);
	get_shared()->cfg.t_die = ft_atoi(argv[2]);
	get_shared()->cfg.t_eat = ft_atoi(argv[3]);
	get_shared()->cfg.t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		get_shared()->cfg.n_access = ft_atoi(argv[5]);
	else
		get_shared()->cfg.n_access = -1;
	if (!(get_shared()->cfg.n_threads && get_shared()->cfg.t_die
			&& get_shared()->cfg.t_eat && get_shared()->cfg.t_sleep
			&& get_shared()->cfg.n_access))
		return (0);
	pthread_mutex_init(&get_shared()->m_0xdeadbeef, 0);
	pthread_mutex_init(&get_shared()->m_io, 0);
	i = 0;
	while (i < get_shared()->cfg.n_threads)
		pthread_mutex_init(&get_shared()->m_res[i++], 0);
	get_shared()->t_start = timestamp();
	return (1);
}

void	_print(int id, char const *s)
{
	pthread_mutex_lock(&get_shared()->m_0xdeadbeef);
	if (!(get_shared()->starved))
	{
		pthread_mutex_lock(&get_shared()->m_io);
		printf("%-6lu ", timestamp() - get_shared()->t_start);
		printf("%d ", id);
		printf("%s\n", s);
		pthread_mutex_unlock(&get_shared()->m_io);
	}
	pthread_mutex_unlock(&get_shared()->m_0xdeadbeef);
}

void	*supervising_thread(void *n)
{
	t_supervisor	*spv;
	int				i;
	int				one_dead;

	spv = get_supervisor();
	one_dead = 0;
	while (!one_dead)
	{
		isleep(2);
		i = -1;
		while (++i < spv->n_threads)
		{
			pthread_mutex_lock(&spv->threads[i].m_res_usage);
			if (timestamp()
				- spv->threads[i].t_last_access >= get_shared()->cfg.t_die)
			{
				pthread_mutex_unlock(&spv->threads[i].m_res_usage);
				one_dead = 1;
				_print(i + 1, "died");
				pthread_mutex_lock(&get_shared()->m_0xdeadbeef);
				get_shared()->starved = 1;
				pthread_mutex_unlock(&get_shared()->m_0xdeadbeef);
				break ;
			}
			pthread_mutex_unlock(&spv->threads[i].m_res_usage);
		}
	}
	return (0);
}

static void	unlock_keep_order(pthread_mutex_t *m0, pthread_mutex_t *m1, int id)
{
	if (get_shared()->cfg.n_threads % 2 && id % 2 == 0
		|| get_shared()->cfg.n_threads % 2 == 0 && id % 2)
	{
		pthread_mutex_unlock(m0);
		pthread_mutex_unlock(m1);
		return ;
	}
	pthread_mutex_unlock(m1);
	pthread_mutex_unlock(m0);
}

static void	lock_keep_order(pthread_mutex_t *m0, pthread_mutex_t *m1, int id)
{
	if (get_shared()->cfg.n_threads % 2 && id % 2 == 0
		|| get_shared()->cfg.n_threads % 2 == 0 && id % 2)
	{
		pthread_mutex_lock(m0);
		_print(id, "has taken a fork");
		pthread_mutex_lock(m1);
		_print(id, "has taken a fork");
		return ;
	}
	pthread_mutex_lock(m1);
	_print(id, "has taken a fork");
	pthread_mutex_lock(m0);
	_print(id, "has taken a fork");
}

int	eat(t_thread *thread)
{
	lock_keep_order(thread->m_res[0], thread->m_res[1], thread->h_id);
	_print(thread->h_id, "is eating");
	pthread_mutex_lock(&thread->m_res_usage);
	isleep(get_shared()->cfg.t_eat);
	thread->t_last_access = timestamp();
	pthread_mutex_unlock(&thread->m_res_usage);
	unlock_keep_order(thread->m_res[0], thread->m_res[1], thread->h_id);
	thread->n_access++;
	if (thread->n_access >= get_shared()->cfg.n_access)
		return (1);
	return (0);
}

void	*routine(void *thrd)
{
	t_thread	*thread;
	int			starved;

	thread = (t_thread *)thrd;
	pthread_mutex_lock(&thread->m_res_usage);
	thread->t_last_access = timestamp();
	pthread_mutex_unlock(&thread->m_res_usage);
	if (get_shared()->cfg.n_threads % 2 == 0 && thread->h_id % 2)
		isleep(1);
	else if (get_shared()->cfg.n_threads % 2 && (thread->h_id == 1 || thread->h_id % 2 == 0))
		isleep(get_shared()->cfg.t_eat + get_shared()->cfg.t_sleep);
	pthread_mutex_lock(&thread->m_res_usage);
	thread->t_last_access = timestamp();
	pthread_mutex_unlock(&thread->m_res_usage);
	starved = 0;
	while (!starved)
	{
		eat(thread);
		_print(thread->h_id, "is sleeping");
		isleep(get_shared()->cfg.t_sleep);
		_print(thread->h_id, "is thinking");
		pthread_mutex_lock(&get_shared()->m_0xdeadbeef);
		starved = get_shared()->starved;
		pthread_mutex_unlock(&get_shared()->m_0xdeadbeef);
	}
	return (thread);
}

int	main(int argc, char **argv)
{
	int					i;
	static pthread_t	spv_id = 0;

	if (argc != 5 && argc != 6)
		return (1);
	if (!init_shared(argc, argv))
		return (1);
	get_supervisor()->n_threads = get_shared()->cfg.n_threads;
	threads_init();
	i = -1;
	while (++i < get_supervisor()->n_threads)
		if (pthread_create(&get_supervisor()->threads[i].tid, 0, routine,
				&get_supervisor()->threads[i]))
			return (EXIT_FAILURE);
	pthread_create(&spv_id, 0, supervising_thread, 0);
	;
	i = -1;
	while (++i < get_supervisor()->n_threads)
		pthread_join(get_supervisor()->threads[i].tid, 0);
	pthread_join(spv_id, 0);
	return (EXIT_SUCCESS);
}
