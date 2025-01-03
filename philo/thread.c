/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   thread.c                                             ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/03 10:38:09 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/03 10:55:22 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	eat(t_thread *thread)
{
	pthread_mutex_lock(thread->m_res[0]);
	_print(thread->shared, thread->h_id, "has taken a fork");
	pthread_mutex_lock(thread->m_res[1]);
	_print(thread->shared, thread->h_id, "has taken a fork");
	pthread_mutex_lock(thread->shared->m_res_usage);
	_print(thread->shared, thread->h_id, "is eating");
	thread->t_last_access = timestamp();
	pthread_mutex_unlock(thread->shared->m_res_usage);
	isleep(thread->shared->config->t_eat);
	pthread_mutex_unlock(thread->m_res[1]);
	pthread_mutex_unlock(thread->m_res[0]);
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
			thread->shared->starved = thread->h_id;
			break ;
		}
		if (eat(thread))
			break ;
		_print(thread->shared, thread->h_id, "is sleeping");
		isleep(thread->shared->config->t_sleep);
		_print(thread->shared, thread->h_id, "is thinking");
	}
	if (thread->starved)
		_print(thread->shared, thread->shared->starved, "died");
	return (thread);
}
