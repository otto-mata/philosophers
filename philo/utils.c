/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   utils.c                                              ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/03 10:37:13 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/03 10:49:03 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	timestamp(void)
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

void	_print(t_shared *shared, int id, char const *s)
{
	if (!(shared->starved))
	{
		pthread_mutex_lock(shared->m_io);
		printf("%-6li ", timestamp() - shared->t_start);
		printf("%d ", id + 1);
		printf("%s\n", s);
		pthread_mutex_unlock(shared->m_io);
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
