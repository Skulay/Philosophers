/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_sleep.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:20 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 22:51:38 by alehamad         ###   ########.fr       */
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
