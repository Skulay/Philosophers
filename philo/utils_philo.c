/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:01:18 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 15:03:21 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int is_dead(t_data *data)
{
	int dead;

	pthread_mutex_lock(&data->death);
	dead = data->dead;
	pthread_mutex_unlock(&data->death);
	return dead;
}

static int	take_forks(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	while (!is_dead(philo->data))
	{
		if (pthread_mutex_trylock(first) == 0)
		{
			if (pthread_mutex_trylock(second) == 0)
			{
				write_philo(philo, FORK);
				write_philo(philo, FORK);
				return (1);
			}
			pthread_mutex_unlock(first);
		}
		// if (philo->id % 2 == 0)
		// 	usleep(300);
		// else
		// 	usleep(200);
	}
	return (0);
}

void	eat_philo(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->left < philo->right)
	{
		first = philo->left;
		second = philo->right;
	}
	else
	{
		first = philo->right;
		second = philo->left;
	}
	if (!take_forks(philo, first, second))
		return ;
	pthread_mutex_lock(&philo->meal);
	philo->last_meal = my_get_time();
	printf("Philo a manger il y a %lld\n", philo->last_meal);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal);
	write_philo(philo, EAT);
	my_usleep(philo->data->t_to_eat);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}
