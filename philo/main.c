/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.c                                               ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/03 08:09:49 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/07 08:30:09 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	usage(char const *exec)
{
	printf("Usage: %s ", exec);
	printf("<number_of_philosophers> ");
	printf("<time_to_die> ");
	printf("<time_to_eat> ");
	printf("<time_to_sleep> ");
	printf("[number_of_times_each_philosopher_must_eat]\n");
	return (EXIT_FAILURE);
}

void	mutex_destroy(pthread_mutex_t *mutex)
{
	pthread_mutex_destroy(mutex);
	free(mutex);
}

int	clean_exit(t_shared **shared, t_thread **threads, int ret)
{
	int	i;

	i = -1;
	while (++i < (*shared)->config->n_threads)
	{
		mutex_destroy((*shared)->m_res[i]);
	}
	free((*shared)->m_res);
	free((*shared)->config);
	mutex_destroy((*shared)->m_io);
	mutex_destroy((*shared)->m_res_usage);
	free(*shared);
	*shared = 0;
	free(*threads);
	*threads = 0;
	return (ret);
}

int	main(int argc, char **argv)
{
	t_thread	*threads;
	t_shared	*shared;
	int			i;

	if (argc != 5 && argc != 6)
		return (usage(argv[0]));
	shared = shared_init(argv);
	if (!shared)
		return (1);
	threads = threads_init(shared);
	if (!threads)
		return (1);
	shared->t_start = timestamp();
	i = -1;
	while (++i < shared->config->n_threads)
		if (pthread_create(&threads[i].tid, 0, routine, &threads[i]))
			return (clean_exit(&shared, &threads, EXIT_FAILURE));
	i = -1;
	while (++i < shared->config->n_threads)
		pthread_join(threads[i].tid, 0);
	return (clean_exit(&shared, &threads, EXIT_SUCCESS));
}
