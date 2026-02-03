/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:09 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 22:51:31 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	data->philo = malloc(sizeof(t_philo) * data->nbr_philo);
	if (!data->philo)
		return (0);
	while (i < data->nbr_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].eat_count = 0;
		data->philo[i].last_meal = 0;
		data->philo[i].left = &data->forks[i];
		data->philo[i].right = &data->forks[(i + 1) % data->nbr_philo];
		data->philo[i].data = data;
		if (pthread_mutex_init(&data->philo[i].meal, NULL) != 0)
			return (0);
		data->meal_count++;
		i++;
	}
	return (1);
}

static int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	if (!data->forks)
		return (0);
	while (i < data->nbr_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (0);
		data->mutex_count++;
		i++;
	}
	return (1);
}

static int	init_mutex(t_data *data)
{
	if (pthread_mutex_init(&data->print, NULL) != 0)
		return (0);
	data->print_ok = 1;
	if (pthread_mutex_init(&data->death, NULL) != 0)
		return (0);
	data->death_ok = 1;
	if (pthread_mutex_init(&data->time, NULL) != 0)
		return (0);
	data->time_ok = 1;
	return (1);
}

static int	init_data(t_data *data, char **av, int eat)
{
	int	i;

	data->nbr_philo = ft_atoi(av[1]);
	data->t_to_die = ft_atoi(av[2]);
	data->t_to_eat = ft_atoi(av[3]);
	data->t_to_sleep = ft_atoi(av[4]);
	i = (data->t_to_die - (data->t_to_eat + data->t_to_sleep)) - 20;
	data->t_to_think = i;
	if (data->t_to_think < 0)
		data->t_to_think = 0;
	if (eat)
		data->nbr_eat = ft_atoi(av[5]);
	else
		data->nbr_eat = -1;
	data->t_start = 0;
	data->forks = NULL;
	data->philo = NULL;
	data->dead = 0;
	data->death_ok = 0;
	data->print_ok = 0;
	data->time_ok = 0;
	data->meal_count = 0;
	data->threads_count = 0;
	data->mutex_count = 0;
	return (1);
}

int	init_all(t_data *data, int ac, char **av)
{
	if (ac == 6)
	{
		if (!init_data(data, av, 1))
			return (0);
	}
	else
	{
		if (!init_data(data, av, 0))
			return (0);
	}
	if (!verif_data(data, ac))
		return (0);
	if (!init_forks(data))
		return (0);
	if (!init_mutex(data))
		return (0);
	if (!init_philo(data))
		return (0);
	return (1);
}
