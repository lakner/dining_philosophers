/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_die.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:13:29 by slakner           #+#    #+#             */
/*   Updated: 2023/01/13 22:42:07 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philosophers.h"

long	timestamp(t_sim *sim)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 * 1000 + tv.tv_usec - sim->time_start);
}

void	wait_for(t_sim *sim, int micros)
{
	int	end_time;

	end_time = timestamp(sim) + micros;
	while (timestamp(sim) < end_time)
		usleep(1);
}

int	die(t_philo *philo, int time_wait)
{
	if (time_wait < 0)
		time_wait = 0;
	wait_for(philo->sim, time_wait);
	philo->dead = 1;
	philo->sim->philo_dead = 1;
	printf("%012ld %d died.\n", timestamp(philo->sim)/1000, philo->n);
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

int	think(t_philo *philo)
{
	if (kick_the_bucket(philo, 0))
		return (1);
	if (philo->activity != THINKING)
	{
		printf("%012ld %d is thinking.\n", timestamp(philo->sim)/1000, philo->n);
		philo->activity = THINKING;
	}
	while ((!*(philo->fork_left) || !*(philo->fork_right)))// || !hungry(philo))
	{
		if (kick_the_bucket(philo, 0))
			return (1);
		wait_for(philo->sim, 1);
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
	if (kick_the_bucket(philo, 0))
		return (1);
	printf("%012ld %d has taken a fork.\n", timestamp(philo->sim)/1000, philo->n);
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
	printf("%012ld %d is eating.\n", eat_time/1000, philo->n);
	if (kick_the_bucket(philo, time_to_eat))
		return(1);
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

int	eat(t_philo *philo)
{
	int		left_idx;
	int		right_idx;

	left_idx = philo->n - 1;
	right_idx = philo->n - 2;
	if (kick_the_bucket(philo, 0))
		return (1);
	if (right_idx < 0)
		right_idx += philo->sim->num_philos;
	if (right_idx == left_idx)
		return (die(philo, philo->time_to_die));
	if (philo->n % 2)
		return (indulge_gluttony(philo, right_idx, left_idx));
	else
		return (indulge_gluttony(philo, left_idx, right_idx));
}

int	nap(t_philo *philo)
{
	int	time_sleep;

	if (kick_the_bucket(philo, 0))
		return (1);
	time_sleep = philo->sim->time_sleep;
	if (philo->activity == EATING)
	{
		philo->activity = SLEEPING;
		printf("%012ld %d is sleeping.\n", timestamp(philo->sim)/1000, philo->n);
		if (kick_the_bucket(philo, philo->sim->time_sleep))
			return (1);
		wait_for(philo->sim, time_sleep);
	}
	return (0);
}

int	feast(t_philo *philo)
{
	if (kick_the_bucket(philo, 0))
		return (1);
	if (think(philo))
		return (1);
	if (kick_the_bucket(philo, 0))
		return (1);
	if (eat(philo))
		return (1);
	if (kick_the_bucket(philo, 0))
		return (1);
	if (philo->ate_n_times == philo->sim->must_eat_times)
	{
		pthread_mutex_lock(&(philo->sim->m_full));
		philo->sim->num_philos_full ++;
		pthread_mutex_unlock(&(philo->sim->m_full));
		return (1);
	}
	if (nap(philo))
		return (1);
	return (0);
}

void	*eat_sleep_die(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (!(philo->sim->sim_has_started))
		usleep(200);
	while (philo && philo->sim && !kick_the_bucket(philo, 0))
	{
		if (feast(philo))
			return (NULL);
	}
	return (NULL);
}
