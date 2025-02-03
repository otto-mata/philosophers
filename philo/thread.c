/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   thread.c                                             ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/03 10:38:09 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/24 14:38:57 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	unlock_keep_order(pthread_mutex_t *m0, pthread_mutex_t *m1)
{
	if (m0 > m1)
	{
		pthread_mutex_unlock(m0);
		pthread_mutex_unlock(m1);
		return ;
	}
	pthread_mutex_unlock(m1);
	pthread_mutex_unlock(m0);
}

static void	lock_keep_order(pthread_mutex_t *m0, pthread_mutex_t *m1,
		t_thread *thread)
{
	if (m0 > m1)
	{
		pthread_mutex_lock(m0);
		_print(thread->shared, thread->h_id, "has taken a fork");
		pthread_mutex_lock(m1);
		_print(thread->shared, thread->h_id, "has taken a fork");
		return ;
	}
	pthread_mutex_lock(m1);
	_print(thread->shared, thread->h_id, "has taken a fork");
	pthread_mutex_lock(m0);
	_print(thread->shared, thread->h_id, "has taken a fork");
}

int	eat(t_thread *thread)
{
	lock_keep_order(thread->m_res[0], thread->m_res[1], thread);
	pthread_mutex_lock(thread->shared->m_res_usage);
	_print(thread->shared, thread->h_id, "is eating");
	isleep(thread->shared->config->t_eat);
	thread->t_last_access = timestamp();
	pthread_mutex_unlock(thread->shared->m_res_usage);
	unlock_keep_order(thread->m_res[0], thread->m_res[1]);
	thread->n_access++;
	if (thread->n_access == thread->shared->config->n_access)
		return (1);
	return (0);
}

void	*routine(void *thrd)
{
	t_thread	*thread;

	thread = (t_thread *)thrd;
	if (thread->h_id % 2)
		isleep(1);
	while (!thread->shared->starved)
	{
		if (thread->t_last_access && delta_t(thread->t_last_access,
				timestamp()) > thread->shared->config->t_die)
		{
			thread->starved = 1;
			pthread_mutex_lock(thread->shared->m_0xdeadbeef);
			thread->shared->starved = thread->h_id + 1;
			pthread_mutex_unlock(thread->shared->m_0xdeadbeef);
			_print(thread->shared, thread->h_id, "died");
			break ;
		}
		if (eat(thread))
			break ;
		_print(thread->shared, thread->h_id, "is sleeping");
		isleep(thread->shared->config->t_sleep);
		_print(thread->shared, thread->h_id, "is thinking");
	}
	return (thread);
}
