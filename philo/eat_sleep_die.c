/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_die.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:13:29 by slakner           #+#    #+#             */
/*   Updated: 2022/12/22 17:04:51 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philosophers.h"

int	curr_timestamp(t_sim *sim)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec/1000 - sim->time_start);
}

void	announce_activity(t_philo *philo)
{
	while (philo->sim->curr != philo->n || !philo->sim->sim_has_started)
		usleep(5);
	pthread_mutex_lock(&(philo->sim->m_curr));
	printf("%09d %d is thinking.\n", curr_timestamp(philo->sim), philo->n);
	philo->sim->curr = (philo->sim->curr % philo->sim->num_philos) + 1; 
	pthread_mutex_unlock(&(philo->sim->m_curr));
}

void	*eat_sleep_die(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	if (philo)
		announce_activity((t_philo *) philo);
	return (NULL);
}
