/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:37:18 by slakner           #+#    #+#             */
/*   Updated: 2022/12/22 00:50:36 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	sim_table(t_sim *sim)
{
	int				i;

	i = 0;
	if (gettimeofday(&(sim->time_start), NULL))
		error_exit(sim, 1, "Error getting system time.");
	prepare_forks(sim);
	release_philos(sim);
	wait_for_the_end(sim);
	free_sim(sim);
	system("leaks philo");
	return (0);
}

t_sim	*init_sim(int num_ph, int time_die, int time_eat, int time_sl)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		error_exit(sim, 2, "Malloc error.");
	sim->num_philos = num_ph;
	sim->curr = 1;
	pthread_mutex_init(&(sim->m_curr), NULL);
	sim->time_die = time_die;
	sim->time_eat = time_eat;
	sim->time_sleep = time_sl;
	return (sim);
}

void	prepare_forks(t_sim *sim)
{
	int	i;

	sim->fork = malloc(sim->num_philos * sizeof(int));
	if (!sim->fork)
		error_exit(sim, 2, "Malloc error.");
	sim->m_fork = malloc(sim->num_philos * sizeof(pthread_mutex_t *));
	i = 0;
	while (i < sim->num_philos)
	{
		sim->fork[i] = 1;
		sim->m_fork[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(sim->m_fork[i], NULL);
		i++;
	}
}

void	prepare_philo(t_sim *sim, int i)
{
	sim->philo[i].n = i + 1;
	sim->philo[i].time_to_die = sim->time_die;
	sim->philo[i].ate_n_times = 0;
	sim->philo[i].fork_left = 0;
	sim->philo[i].fork_right = 0;
	sim->philo[i].dead = 0;
	sim->philo[i].activity = THINKING;
	sim->philo[i].sim = sim;
}

void	release_philos(t_sim *sim)
{
	int	i;

	sim->philo = malloc(sim->num_philos * sizeof(t_philo));
	if (!sim->philo)
		error_exit(sim, 2, "Malloc error.");
	i = 0;
	while (i < sim->num_philos)
	{
		prepare_philo(sim, i);
		pthread_create(&(sim->philo[i].thread), NULL,
			&eat_sleep_die, &(sim->philo[i]));
		i ++;
	}
}

void	wait_for_the_end(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philos)
	{	
		pthread_join(sim->philo[i].thread, NULL);
		i++;
	}
}

void	close_mutexes(t_sim *sim)
{
	int	i;

	i = 0;
	if (sim)
	{
		pthread_mutex_destroy(&(sim->m_curr));
		while (i < sim->num_philos)
		{
			pthread_mutex_destroy(sim->m_fork[i]);
			free(sim->m_fork[i]);
			i++;
		}
	}
}

void	free_sim(t_sim *sim)
{
	close_mutexes(sim);
	if (sim)
	{
		if (sim->philo)
			free(sim->philo);
		if (sim->m_fork)
			free(sim->m_fork);
		if (sim->fork)
			free(sim->fork);
		free(sim);
	}
	return ;
}
