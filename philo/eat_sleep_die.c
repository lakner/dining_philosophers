/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_die.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:13:29 by slakner           #+#    #+#             */
/*   Updated: 2022/12/21 23:21:16 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philosophers.h"

void	announce_activity(t_philo *philo)
{
	while (philo->sim->curr != philo->n)
		usleep(50);
	pthread_mutex_lock(&(philo->sim->m_curr));
	printf("Philosopher %d is thinking.\n", philo->n);
	if (philo->sim->curr == philo->sim->num_philos)
		philo->sim->curr = 1;
	else
		philo->sim->curr ++;
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
