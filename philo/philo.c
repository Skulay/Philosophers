/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:22 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 15:00:06 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo(t_data *data)
{
	data->t_start = my_get_time();
	pthread_mutex_lock(data->philo[0].left);
	write_philo(&data->philo[0], FORK);
	my_usleep(data->t_to_die);
	pthread_mutex_unlock(data->philo[0].left);
	pthread_mutex_lock(&data->death);
	data->dead = 1;
	pthread_mutex_unlock(&data->death);
	write_philo(&data->philo[0], DIED);
}

void	ft_philo(t_data *data)
{
	int	i;

	i = 0;
	if (data->nbr_philo == 1)
	{
		one_philo(data);
		return ;
	}
	data->t_start = my_get_time();
	while (i < data->nbr_philo)
	{
		pthread_mutex_lock(&data->philo[i].meal);
		data->philo[i].last_meal = data->t_start;
		pthread_mutex_unlock(&data->philo[i].meal);
		pthread_create(&data->philo[i].thread, NULL,
			routine_philo, &data->philo[i]);
		i++;
	}
	monitor(data);
	i = 0;
	while (i < data->nbr_philo)
	{
		pthread_join(data->philo[i].thread, NULL);
		i++;
	}
}

void	write_philo(t_philo *philo, int state)
{
	long long	timestamp;
	int			dead;

	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->death);
	dead = philo->data->dead;
	pthread_mutex_unlock(&philo->data->death);
	if (dead && state != DIED)
	{
		pthread_mutex_unlock(&philo->data->print);
		return;
	}
	timestamp = my_get_time() - philo->data->t_start;
	if (state == FORK)
		printf("%lld %d has taken a fork\n", timestamp, philo->id);
	else if (state == EAT)
		printf("%lld %d is eating\n", timestamp, philo->id);
	else if (state == SLEEP)
		printf("%lld %d is sleeping\n", timestamp, philo->id);
	else if (state == THINK)
		printf("%lld %d is thinking\n", timestamp, philo->id);
	else if (state == DIED)
		printf("%lld %d died\n", timestamp, philo->id);
	pthread_mutex_unlock(&philo->data->print);
}
