/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_die.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:13:29 by slakner           #+#    #+#             */
/*   Updated: 2023/01/05 21:10:26 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philosophers.h"

int	timestamp(t_sim *sim)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	//printf("timestamp: %d\n", (tv.tv_usec/1000) - sim->time_start);
	return (tv.tv_sec * 1000 + tv.tv_usec/1000 - sim->time_start);
}

void	announce_activity(t_philo *philo)
{
	while (philo->sim->curr != philo->n || !philo->sim->sim_has_started)
		usleep(1);
	pthread_mutex_lock(&(philo->sim->m_curr));
	if (philo->n == 500)
		printf("%09d %d is thinking.\n", timestamp(philo->sim), philo->n);
	philo->sim->curr = (philo->sim->curr % philo->sim->num_philos) + 1; 
	pthread_mutex_unlock(&(philo->sim->m_curr));
}

int	hungry(t_philo *philo)
{
	if (philo->time_to_die < (philo->last_meal * 3 / 2))
		return (1);
	return (0);
}

void	think(t_philo *philo)
{
	while (!*(philo->fork_left) || !*(philo->fork_right))// || !hungry(philo))
		usleep(1);
}

void	grab_fork(t_philo *philo, int idx)
{
	pthread_mutex_lock(philo->sim->m_fork[idx]);
	printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
	philo->sim->fork[idx] = 0;
}

void	stuff_face(t_philo *philo)
{
	int		eat_time;

	philo->activity = EATING;
	eat_time = timestamp(philo->sim);
	philo->last_meal = eat_time;
	printf("%09d %d is eating.\n", eat_time, philo->n);
	usleep(philo->sim->time_eat * 1000);
	philo->ate_n_times++;
}

void	return_fork(t_philo *philo, int idx)
{
	philo->sim->fork[idx] = 1;
	pthread_mutex_unlock(philo->sim->m_fork[idx]);
}

void	indulge_gluttony(t_philo *philo, int first, int second)
{
	grab_fork(philo, first);
	if (philo->sim->fork[second])
	{
		grab_fork(philo, second);
		stuff_face(philo);
		return_fork(philo, second);
	}
	return_fork(philo, first);
}

void	eat(t_philo *philo)
{
	int		left_idx;
	int		right_idx;
	
	left_idx = philo->n - 1;
	right_idx = philo->n - 2;
	if (right_idx < 0)
		right_idx += philo->sim->num_philos;
	if (philo->n % 2)
	{
		indulge_gluttony(philo, right_idx, left_idx);
		// pthread_mutex_lock(philo->sim->m_fork[right_idx]);
		// *(philo->fork_right) = 0;
		// printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
		// if (philo->fork_left)
		// {
		// 	pthread_mutex_lock(philo->sim->m_fork[left_idx]);
		// 	*(philo->fork_left) = 0;
		// 	printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
		// 	philo->activity = EATING;
		// 	eat_time = timestamp(philo->sim);
		// 	philo->last_meal = eat_time;
		// 	printf("%09d %d is eating.\n", eat_time, philo->n);
		// 	usleep(philo->sim->time_eat * 1000);
		// 	philo->ate_n_times++;
		// 	*(philo->fork_left) = 1;
		// 	pthread_mutex_unlock(philo->sim->m_fork[left_idx]);
		// }
		// *(philo->fork_right) = 1;
		// pthread_mutex_unlock(philo->sim->m_fork[right_idx]);
		philo->activity = SLEEPING;
		printf("%09d %d is sleeping.\n", timestamp(philo->sim), philo->n);
		usleep(philo->sim->time_sleep * 1000);
		philo->activity = THINKING;
	}
	else
	{
		indulge_gluttony(philo, left_idx, right_idx);
		// pthread_mutex_lock(philo->sim->m_fork[left_idx]);
		// *(philo->fork_left) = 0;
		// printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
		// if (philo->fork_right)
		// {
		// 	pthread_mutex_lock(philo->sim->m_fork[right_idx]);
		// 	*(philo->fork_right) = 0;
		// 	printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
		// 	philo->activity = EATING;
		// 	eat_time = timestamp(philo->sim);
		// 	philo->last_meal = eat_time;
		// 	printf("%09d %d is eating.\n", eat_time, philo->n);
		// 	usleep(philo->sim->time_eat * 1000);
		// 	philo->ate_n_times++;
		// 	*(philo->fork_right) = 1;
		// 	pthread_mutex_unlock(philo->sim->m_fork[right_idx]);
		// }
		// *(philo->fork_left) = 1;
		// pthread_mutex_unlock(philo->sim->m_fork[left_idx]);
		philo->activity = SLEEPING;
		printf("%09d %d is sleeping.\n", timestamp(philo->sim), philo->n);
		usleep(philo->sim->time_sleep * 1000);
		philo->activity = THINKING;
	}
}

void	feast(t_philo *philo)
{
	think(philo);
	eat(philo);
}

void	*eat_sleep_die(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (!(philo->sim->sim_has_started))
		usleep(10);
	// if (philo->n % 2 == 0)
	// 	usleep(50);
	while (philo)
	{	
		if (philo->activity == THINKING
			&& (philo->time_to_die - (timestamp(philo->sim) - philo->last_meal) < (philo->sim->time_die * 3/2)))
			feast(philo);
		// else if (philo->activity == EATING)
		// 	sleep();
		// else if (philo->activity == SLEEPING)
		// 	think();
			//announce_activity((t_philo *) philo);
	}
	return (NULL);
}
