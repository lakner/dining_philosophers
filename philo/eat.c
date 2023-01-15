/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:32:25 by slakner           #+#    #+#             */
/*   Updated: 2023/01/15 19:57:32 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	eat(t_philo *philo)
{
	int		left_idx;
	int		right_idx;

	pthread_mutex_lock(&(philo->sim->m_firstlast));
	if (philo->sim->blocked_philo == philo->n)
	{
		pthread_mutex_unlock(&(philo->sim->m_firstlast));
		philo->sim->blocked_philo += 1;
		philo->sim->blocked_philo %= philo->sim->num_philos;
		return (0);
	}
	pthread_mutex_unlock(&(philo->sim->m_firstlast));
	left_idx = philo->n - 1;
	right_idx = philo->n - 2;
	if (right_idx < 0)
		right_idx += philo->sim->num_philos;
	if (right_idx == left_idx)
		return (die(philo, philo->time_to_die));
	if (philo->n % 2)
		return (indulge_gluttony(philo, right_idx, left_idx));
	else
		return (indulge_gluttony(philo, left_idx, right_idx));
}

int	indulge_gluttony(t_philo *philo, int first, int second)
{
	if (first == second)
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
