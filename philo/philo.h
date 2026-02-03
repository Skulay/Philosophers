/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:49:00 by alehamad          #+#    #+#             */
/*   Updated: 2026/02/03 22:51:36 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

# define DIED 1
# define EAT 2
# define SLEEP 3
# define FORK  4
# define THINK 5

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	pthread_mutex_t	meal;
	t_data			*data;
}	t_philo;
//arg
typedef struct s_data
{
	int				nbr_philo;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				t_to_think;
	int				nbr_eat;
	int				dead;
	int				threads_count;
	int				mutex_count;
	int				meal_count;
	int				print_ok;
	int				death_ok;
	int				time_ok;
	long long		t_start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	death;
	pthread_mutex_t	time;
	t_philo			*philo;
}	t_data;

// parsing
int			verif_data(t_data *data, int ac);
int			ft_atoi(const char *nptr);
int			verif_arg(char **av);

// init
int			init_all(t_data *data, int ac, char **av);

// routine et philo
void		one_philo(t_data *data);
void		*routine_philo(void *arg);
void		write_philo(t_philo *philo, int state);
void		ft_philo(t_data *data);
void		monitor(t_data *data);
void		eat_philo(t_philo *philo);

// utils
void		my_usleep(long long duration_ms);
long long	my_get_time(void);
void		my_free_data(t_data *data);

#endif
