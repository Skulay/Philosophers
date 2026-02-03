/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:15 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 15:03:49 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	verif_arg(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	verif_data(t_data *data, int ac)
{
	if (data->nbr_philo <= 0 || data->nbr_philo >= INT_MAX)
		return (0);
	if (data->t_to_die <= 0 || data->t_to_die >= INT_MAX)
		return (0);
	if (data->t_to_eat <= 0 || data->t_to_eat >= INT_MAX)
		return (0);
	if (data->t_to_sleep <= 0 || data->t_to_sleep >= INT_MAX)
		return (0);
	if ((data->nbr_eat <= 0 && ac == 6) || data->nbr_eat >= INT_MAX)
		return (0);
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int		nb;
	size_t	i;
	int		sign;

	i = 0;
	nb = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = (nb * 10) + (nptr[i] - '0');
		i++;
	}
	return (nb * sign);
}
