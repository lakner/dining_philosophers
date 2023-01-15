/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 17:02:10 by slakner           #+#    #+#             */
/*   Updated: 2023/01/15 20:31:37 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_sim	*init_sim(int num_ph, int time_die, int time_eat, int time_sl)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		error_exit(sim, 2, "Malloc error.");
	sim->num_philos = num_ph;
	sim->num_philos_full = 0;
	pthread_mutex_init(&(sim->m_dead), NULL);
	pthread_mutex_init(&(sim->m_full), NULL);
	pthread_mutex_init(&(sim->m_speak), NULL);
	pthread_mutex_init(&(sim->m_start), NULL);
	pthread_mutex_init(&(sim->m_firstlast), NULL);
	sim->time_die = time_die * 1000;
	sim->time_eat = time_eat * 1000;
	sim->time_sleep = time_sl * 1000;
	sim->sim_has_started = 0;
	// if (num_ph % 2)
	// 	sim->blocked_philo = num_ph;
	// else
		sim->blocked_philo = 0;
	return (sim);
}

int	sim_table(t_sim *sim)
{
	prepare_philos(sim);
	prepare_forks(sim);
	release_philos(sim);
	while (!sim->philo_dead && sim->num_philos 
		&& sim->num_philos_full < sim->num_philos)
		;
	wait_for_the_end(sim);
	free_sim(sim);
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
		sim->philo[i].just_ate = 0;
		sim->philo[i].fork_left = NULL;
		sim->philo[i].fork_right = NULL;
		sim->philo[i].has_fork_idx1 = -1;
		sim->philo[i].has_fork_idx2 = -1;
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