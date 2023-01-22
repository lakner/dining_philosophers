/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:29:39 by slakner           #+#    #+#             */
/*   Updated: 2023/01/22 19:20:03 by slakner          ###   ########.fr       */
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
	printf("%012ld %d died.\n", timestamp(philo->sim) / 1000, philo->n);
	return (0);
}

int	kick_the_bucket(t_philo *philo)
{
	pthread_mutex_lock(&(philo->sim->m_dead));
	die(philo,
		(philo->last_meal + philo->time_to_die) - timestamp(philo->sim));
	return_fork(philo,
		(philo->n - 1 + philo->sim->num_philos) % philo->sim->num_philos);
	return_fork(philo,
		(philo->n - 2 + philo->sim->num_philos) % philo->sim->num_philos);
	pthread_mutex_unlock(&(philo->sim->m_dead));
	return (0);
}
