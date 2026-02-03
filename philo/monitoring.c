/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:27 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 22:51:33 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_slayer(t_data *data, int i)
{
	long long	timestamp;

	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death);
	data->dead = 1;
	pthread_mutex_unlock(&data->death);
	timestamp = my_get_time() - data->t_start;
	printf("%lld %d died\n", timestamp, data->philo[i].id);
	pthread_mutex_unlock(&data->print);
}

static int	check_dead(t_data *data, int i)
{
	long long	last;
	long long	now;

	pthread_mutex_lock(&data->philo[i].meal);
	last = data->philo[i].last_meal;
	pthread_mutex_unlock(&data->philo[i].meal);
	now = my_get_time();
	if (now < last)
		now = last;
	return (now - last > data->t_to_die);
}

static int	all_ate(t_data *data)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (data->nbr_eat == -1)
		return (0);
	while (i < data->threads_count)
	{
		pthread_mutex_lock(&data->philo[i].meal);
		count = data->philo[i].eat_count;
		pthread_mutex_unlock(&data->philo[i].meal);
		if (count < data->nbr_eat)
			return (0);
		i++;
	}
	return (1);
}

void	monitor(t_data *data)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < data->threads_count)
		{
			if (check_dead(data, i))
			{
				ft_slayer(data, i);
				return ;
			}
			i++;
		}
		if (all_ate(data))
		{
			pthread_mutex_lock(&data->death);
			data->dead = 1;
			pthread_mutex_unlock(&data->death);
			return ;
		}
		usleep(500);
	}
}
