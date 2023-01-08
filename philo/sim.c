/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:37:18 by slakner           #+#    #+#             */
/*   Updated: 2023/01/08 16:48:14 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	sim_table(t_sim *sim)
{
	prepare_philos(sim);
	prepare_forks(sim);
	release_philos(sim);
	// while (!sim->philo_dead)
	// 	;
	wait_for_the_end(sim);
	free_sim(sim);
	system("leaks philo");
	return (0);
}

void	prepare_philos(t_sim *sim)
{
	int	i;

	i = 0;
	sim->philo = malloc(sim->num_philos * sizeof(t_philo));
	if (!sim->philo)
		error_exit(sim, 2, "Malloc error.");
	while (i < sim->num_philos)
	{
		sim->philo[i].n = i + 1;
		sim->philo[i].time_to_die = sim->time_die;
		sim->philo[i].ate_n_times = 0;
		sim->philo[i].fork_left = NULL;
		sim->philo[i].fork_right = NULL;
		sim->philo[i].dead = 0;
		sim->philo[i].last_meal = 0;
		sim->philo[i].activity = SLEEPING;
		sim->philo[i].sim = sim;
		i++;
	}
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
		sim->philo[i].fork_left = &(sim->fork[i]);
		sim->philo[(i + 1) % sim->num_philos].fork_right = &(sim->fork[i]);
		sim->m_fork[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(sim->m_fork[i], NULL);
		i++;
	}
}

void	release_philos(t_sim *sim)
{
	int				i;
	struct timeval	tv;

	i = 0;
	while (i < sim->num_philos)
	{
		pthread_create(&(sim->philo[i].thread), NULL,
			&eat_sleep_die, &(sim->philo[i]));
		i ++;
	}
	if (gettimeofday(&tv, NULL))
		error_exit(sim, 1, "Error getting system time.");
	sim->time_start = tv.tv_sec * 1000 + tv.tv_usec/1000;
	sim->sim_has_started = 1;
}

void	wait_for_the_end(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philos) // && !sim->philo_dead)
	{	
		//pthread_detach(sim->philo[i].thread);
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
		// pthread_mutex_destroy(&(sim->m_curr));
		pthread_mutex_destroy(&(sim->m_dead));
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
