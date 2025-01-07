/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   utils1.c                                             ┌─┐┌┬┐┌┬┐┌─┐        */
/*                                                        │ │ │  │ │ │        */
/*   By: tblochet <tblochet@student.42.fr>                └─┘ ┴  ┴ └─┘        */
/*                                                        ┌┬┐┌─┐┌┬┐┌─┐        */
/*   Created: 2025/01/07 08:12:16 by tblochet             │││├─┤ │ ├─┤        */
/*   Updated: 2025/01/07 08:24:19 by tblochet             ┴ ┴┴ ┴ ┴ ┴ ┴        */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_valid_int32(char const *nptr)
{
	int		sign;
	long	res;

	sign = 1;
	res = 0;
	while (*nptr == ' ' || (*nptr >= '\t' && *nptr <= '\r'))
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
	if (res * sign > INT32_MAX || res * sign < INT32_MIN)
		return (0);
	return (1);
}

int	pos_int32_atoi(char const *nptr)
{
	if (!is_valid_int32(nptr))
		return (-1);
	return (ft_atoi(nptr));
}
