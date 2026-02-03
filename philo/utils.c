/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:01:18 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 22:51:39 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_data *data)
{
	int	dead;

	pthread_mutex_lock(&data->death);
	dead = data->dead;
	pthread_mutex_unlock(&data->death);
	return (dead);
}

int	take_forks(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	if (is_dead(philo->data))
		return (0);
	pthread_mutex_lock(first);
	write_philo(philo, FORK);
	if (is_dead(philo->data))
	{
		pthread_mutex_unlock(first);
		return (0);
	}
	pthread_mutex_lock(second);
	write_philo(philo, FORK);
	if (is_dead(philo->data))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return (0);
	}
	return (1);
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
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal);
	write_philo(philo, EAT);
	my_usleep(philo->data->t_to_eat);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}
