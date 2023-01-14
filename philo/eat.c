/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:32:25 by slakner           #+#    #+#             */
/*   Updated: 2023/01/14 18:49:37 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// checks if philo just ate
int	hungry(t_philo *philo)
{
	if (philo->last_meal
		&& (timestamp(philo->sim)
			- (philo->last_meal + philo->sim->time_eat + philo->sim->time_sleep)
			<= 200)
	)
		return (0);
	return (1);
}

int	grab_fork(t_philo *philo, int idx)
{
	pthread_mutex_lock(philo->sim->m_fork[idx]);
	if (philo->has_fork_idx1 < 0)
		philo->has_fork_idx1 = idx;
	else
		philo->has_fork_idx2 = idx;
	say(philo, timestamp(philo->sim) / 1000, "has taken a fork.");
	philo->sim->fork[idx] = 0;
	return (0);
}

int	stuff_face(t_philo *philo)
{
	long		eat_time;
	long		time_to_eat;

	if (kick_the_bucket(philo, 0))
		return (1);
	philo->activity = EATING;
	eat_time = timestamp(philo->sim);
	time_to_eat = philo->sim->time_eat;
	philo->last_meal = eat_time;
	philo->just_ate = 1;
	say(philo, timestamp(philo->sim) / 1000, "is eating.");
	if (kick_the_bucket(philo, time_to_eat))
		return (1);
	wait_for(philo->sim, time_to_eat);
	philo->ate_n_times++;
	return (0);
}

int	return_fork(t_philo *philo, int idx)
{
	philo->sim->fork[idx] = 1;
	if (philo->has_fork_idx1 >= 0)
		philo->has_fork_idx1 = -1;
	else
		philo->has_fork_idx2 = -1;
	pthread_mutex_unlock(philo->sim->m_fork[idx]);
	return (0);
}

int	indulge_gluttony(t_philo *philo, int first, int second)
{
	if (kick_the_bucket(philo, 0) || (first == second))
		return (1);
	if (philo->sim->fork[first] && philo->sim->fork[second])
	{
		if (grab_fork(philo, first) || grab_fork(philo, second)
			|| stuff_face(philo)
			|| return_fork(philo, second) || return_fork(philo, first))
			return (1);
	}
	return (0);
}
