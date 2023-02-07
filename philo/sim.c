/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:37:18 by slakner           #+#    #+#             */
/*   Updated: 2023/02/03 19:33:17 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	release_philos(t_sim *sim)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&(sim->m_start));
	while (i < sim->num_philos)
	{
		pthread_create(&(sim->philo[i].thread), NULL,
			&eat_sleep_die, &(sim->philo[i]));
		i ++;
	}
	sim->time_start = 0;
	sim->time_start = timestamp(sim);
	sim->sim_has_started = 1;
	pthread_mutex_unlock(&(sim->m_start));
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
		pthread_mutex_destroy(&(sim->m_dead));
		pthread_mutex_destroy(&(sim->m_full));
		pthread_mutex_destroy(&(sim->m_start));
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
