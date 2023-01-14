/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:29:39 by slakner           #+#    #+#             */
/*   Updated: 2023/01/14 18:30:34 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	die(t_philo *philo, int time_wait)
{
	if (time_wait < 0)
		time_wait = 0;
	wait_for(philo->sim, time_wait);
	philo->dead = 1;
	philo->sim->philo_dead = 1;
	say(philo, timestamp(philo->sim)/1000, "died.");
	return (0);
}

int	kick_the_bucket(t_philo *philo, int time_wait)
{
	pthread_mutex_lock(&(philo->sim->m_dead));
	if (philo->sim->philo_dead)
	{
		pthread_mutex_unlock(&(philo->sim->m_dead));
		if (philo->has_fork_idx1 >= 0)
			return_fork(philo, philo->has_fork_idx1);
		if (philo->has_fork_idx2 >= 0)
			return_fork(philo, philo->has_fork_idx2);
		return (1);
	}
	if ((timestamp(philo->sim) + time_wait)
		> (philo->last_meal + philo->time_to_die))
	{
		die(philo,
			(philo->last_meal + philo->time_to_die) - timestamp(philo->sim));
		pthread_mutex_unlock(&(philo->sim->m_dead));
		if (philo->has_fork_idx1 >= 0)
			return_fork(philo, philo->has_fork_idx1);
		if (philo->has_fork_idx2 >= 0)
			return_fork(philo, philo->has_fork_idx2);
		return (1);
	}
	pthread_mutex_unlock(&(philo->sim->m_dead));
	return (0);
}