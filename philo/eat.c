/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:32:25 by slakner           #+#    #+#             */
/*   Updated: 2023/01/22 17:44:41 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	eat(t_philo *philo)
{
	int		left_idx;
	int		right_idx;

	left_idx = philo->n - 1;
	right_idx = philo->n - 2;
	if (right_idx < 0)
		right_idx += philo->sim->num_philos;
	if (right_idx == left_idx)
		return (1);
	return (indulge_gluttony(philo, left_idx, right_idx));
}

int	indulge_gluttony(t_philo *philo, int first, int second)
{
	int	ret;

	if (first == second)
		return (1);
	grab_fork(philo, first);
	grab_fork(philo, second);
	ret = stuff_face(philo);
	return_fork(philo, second);
	return_fork(philo, first);
	return (ret);
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
	long		time_to_eat;

	time_to_eat = philo->sim->time_eat;
	pthread_mutex_lock(&(philo->sim->m_full));
	philo->last_meal = timestamp(philo->sim);
	pthread_mutex_unlock(&(philo->sim->m_full));
	say(philo, timestamp(philo->sim) / 1000, "is eating.");
	wait_for(philo->sim, time_to_eat);
	pthread_mutex_lock(&(philo->sim->m_full));
	philo->ate_n_times++;
	pthread_mutex_unlock(&(philo->sim->m_full));
	return (0);
}

int	return_fork(t_philo *philo, int idx)
{
	pthread_mutex_lock(&(philo->sim->m_full));
	philo->sim->fork[idx] = 1;
	if (philo->has_fork_idx1 >= 0)
		philo->has_fork_idx1 = -1;
	else
		philo->has_fork_idx2 = -1;
	pthread_mutex_unlock(&(philo->sim->m_full));
	pthread_mutex_unlock(philo->sim->m_fork[idx]);
	return (0);
}
