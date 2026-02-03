/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:27 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 15:00:08 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void monitor(t_data *data)
{
	int i;
	long long now, last;

	while (1)
	{
		i = 0;
		while (i < data->nbr_philo)
		{
			pthread_mutex_lock(&data->philo[i].meal);
			last = data->philo[i].last_meal;
			pthread_mutex_unlock(&data->philo[i].meal);
			now = my_get_time();
			if (now < last)
				now = last;
			if (now - last > data->t_to_die)
			{
				pthread_mutex_lock(&data->death);
				data->dead = 1;
				pthread_mutex_unlock(&data->death);
				write_philo(&data->philo[i], DIED);
				return;
			}
			i++;
		}
	}
}
