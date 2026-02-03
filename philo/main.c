/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:40:00 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 22:51:32 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac != 5 && ac != 6)
		return (1);
	if (!verif_arg(av))
		return (1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	if (!init_all(data, ac, av))
	{
		my_free_data(data);
		return (1);
	}
	ft_philo(data);
	my_free_data(data);
	return (0);
}
