/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 20:24:54 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 22:51:30 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_pthjoin(t_data *data)
{
	int	i;

	if (!data || !data->philo)
		return ;
	i = 0;
	while (i < data->threads_count)
	{
		pthread_join(data->philo[i].thread, NULL);
		i++;
	}
}

void	my_pthread_destroy_helper(t_data *data)
{
	int	i;

	i = 0;
	if (data->forks)
	{
		i = 0;
		while (i < data->mutex_count)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
		data->forks = NULL;
	}
	if (data->print_ok)
		pthread_mutex_destroy(&data->print);
	if (data->death_ok)
		pthread_mutex_destroy(&data->death);
	if (data->time_ok)
		pthread_mutex_destroy(&data->time);
}

void	my_pthread_destroy(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->philo)
	{
		i = 0;
		while (i < data->meal_count)
		{
			pthread_mutex_destroy(&data->philo[i].meal);
			i++;
		}
		free(data->philo);
		data->philo = NULL;
	}
	my_pthread_destroy_helper(data);
}

void	my_free_data(t_data *data)
{
	if (!data)
		return ;
	ft_pthjoin(data);
	my_pthread_destroy(data);
	free(data);
}
