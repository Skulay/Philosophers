/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:25 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 22:51:37 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->death);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death);
		eat_philo(philo);
		write_philo(philo, SLEEP);
		my_usleep(philo->data->t_to_sleep);
		write_philo(philo, THINK);
		usleep(philo->data->t_to_think * 1000);
	}
	return (NULL);
}
