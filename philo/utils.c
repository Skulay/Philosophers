/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:20 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 15:00:10 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	my_get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	my_usleep(long long duration_ms)
{
	long long	start;
	long long	now;
	long long	left;

	start = my_get_time();
	while (1)
	{
		now = my_get_time();
		left = duration_ms - (now - start);
		if (left <= 0)
			break ;
		if (left > 10)
			usleep(1000);
		else
			usleep(100);
	}
}

void	my_pthread_destroy(t_data *data)
{
	int	i;

	i = 0;
	if (data->philo)
	{
		while (i < data->nbr_philo)
		{
			pthread_mutex_destroy(&data->philo[i].meal);
			i++;
		}
		free(data->philo);
	}
	i = 0;
	if (data->forks)
	{
		while (i < data->nbr_philo)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->time);
}

void	my_free_data(t_data *data)
{
	if (!data)
		return ;
	my_pthread_destroy(data);
	free(data);
}
