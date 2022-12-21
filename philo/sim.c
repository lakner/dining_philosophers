/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:37:18 by slakner           #+#    #+#             */
/*   Updated: 2022/12/21 21:29:36 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	sim_table(t_sim *sim)
{
	int				i;

	i = 0;
	if (gettimeofday(&(sim->time_start), NULL))
		error_exit(sim, 1, "Error getting system time.");
	release_philos(sim);
	free_sim(sim);
	system("leaks philo");
	return (0);
}

t_sim	*init_sim(int num_ph, int time_die, int time_eat, int time_sl)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (NULL);
	sim->philo = malloc(num_ph * sizeof(t_philo));
	if (!sim->philo)
		error_exit(sim, 2, "Malloc error.");
	sim->fork = malloc(num_ph * sizeof(int));
	if (!sim->fork)
		error_exit(sim, 2, "Malloc error.");
	sim->num_philos = num_ph;
	sim->time_die = time_die;
	sim->time_eat = time_eat;
	sim->time_sleep = time_sl;
	create_mutexes(sim);
	release_philos(sim);
	return (sim);
}

void	create_mutexes(t_sim *sim)
{
	int	i;

	i = 0;
	pthread_mutex_init(&(sim->m_announcing), NULL);
	sim->m_fork = malloc(sim->num_philos * sizeof(pthread_mutex_t *));
	while (i < sim->num_philos)
		pthread_mutex_init(&(sim->m_fork[i++]), NULL);
}

void	release_philos(t_sim *sim)
{
	pthread_t		thread;
	int				i;

	i = 0;
	while (i < sim->num_philos)
	{
		sim->philo[i].n = i + 1;
		sim->philo[i].time_to_die = sim->time_die;
		sim->philo[i].ate_n_times = 0;
		sim->philo[i].fork_left = 0;
		sim->philo[i].fork_right = 0;
		sim->philo[i].activity = THINKING;
		pthread_create(&thread, NULL, eat_sleep_die, &(sim->philo[i]));
		i ++;
	}
}

void	*free_sim(t_sim *sim)
{
	if (sim)
	{
		if (sim->philo)
			free(sim->philo);
		if (sim->fork)
			free(sim->fork);
		free(sim);
	}
	return (NULL);
}
