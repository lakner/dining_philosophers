/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:32:25 by slakner           #+#    #+#             */
/*   Updated: 2023/02/03 19:44:19 by slakner          ###   ########.fr       */
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
	if (first > second)
	{
		grab_fork(philo, second);
		grab_fork(philo, first);
		ret = stuff_face(philo);
		return_fork(philo, first);
		return_fork(philo, second);
	}
	else
	{
		grab_fork(philo, second);
		grab_fork(philo, first);
		ret = stuff_face(philo);
		return_fork(philo, first);
		return_fork(philo, second);
	}
	return (ret);
}

int	grab_fork(t_philo *philo, int idx)
{
	pthread_mutex_lock(philo->sim->m_fork[idx]);
	say(philo, timestamp(philo->sim) / 1000, "has taken a fork.");
	return (0);
}

int	stuff_face(t_philo *philo)
{
	long	time_to_eat;
	long	mealtime;	

	pthread_mutex_lock(&(philo->sim->m_full));
	time_to_eat = philo->sim->time_eat;
	philo->last_meal = timestamp(philo->sim);
	mealtime = philo->last_meal;
	pthread_mutex_unlock(&(philo->sim->m_full));
	say(philo, mealtime / 1000, "is eating.");
	wait_for(philo->sim, time_to_eat);
	pthread_mutex_lock(&(philo->sim->m_full));
	philo->ate_n_times++;
	pthread_mutex_unlock(&(philo->sim->m_full));
	return (0);
}

int	return_fork(t_philo *philo, int idx)
{
	pthread_mutex_unlock(philo->sim->m_fork[idx]);
	return (0);
}
