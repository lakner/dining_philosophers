/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_die.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:13:29 by slakner           #+#    #+#             */
/*   Updated: 2023/02/03 19:39:32 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	say(t_philo *philo, long timestamp, char *msg)
{
	pthread_mutex_lock(&(philo->sim->m_dead));
	if (!philo->sim->philo_dead)
		printf("%012ld %d %s\n", timestamp, philo->n, msg);
	pthread_mutex_unlock(&(philo->sim->m_dead));
}

int	think(t_philo *philo)
{
	say(philo, timestamp(philo->sim) / 1000, "is thinking.");
	return (0);
}

int	nap(t_philo *philo)
{
	say(philo, timestamp(philo->sim) / 1000, "is sleeping.");
	wait_for(philo->sim, philo->sim->time_sleep);
	return (0);
}

int	feast(t_philo *philo)
{
	pthread_mutex_lock(&(philo->sim->m_dead));
	if (philo->sim->philo_dead)
	{
		pthread_mutex_unlock(&(philo->sim->m_dead));
		return (1);
	}
	pthread_mutex_unlock(&(philo->sim->m_dead));
	if (eat(philo))
		return (1);
	pthread_mutex_lock(&(philo->sim->m_full));
	if (philo->ate_n_times == philo->sim->must_eat_times)
	{
		philo->sim->num_philos_full ++;
		pthread_mutex_unlock(&(philo->sim->m_full));
		return (0);
	}
	pthread_mutex_unlock(&(philo->sim->m_full));
	if (nap(philo))
		return (1);
	if (think(philo))
		return (1);
	return (0);
}

void	*eat_sleep_die(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(&(philo->sim->m_start));
	pthread_mutex_unlock(&(philo->sim->m_start));
	while (!(philo->sim->sim_has_started))
		wait_for(philo->sim, 1);
	if (!(philo->n % 2))
		wait_for(philo->sim, philo->sim->time_eat / 10);
	while (philo && philo->sim)
	{
		if (feast(philo))
			return (NULL);
		if (philo->ate_n_times == philo->sim->must_eat_times)
			break ;
	}
	return (NULL);
}
