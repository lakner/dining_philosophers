/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_die.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:13:29 by slakner           #+#    #+#             */
/*   Updated: 2022/12/22 19:26:42 by slakner          ###   ########.fr       */
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

//void	die()

void	eat(t_philo *philo)
{
	int		left_idx;
	int		right_idx;
	int		eat_time;

	left_idx = philo->n - 1;
	right_idx = philo->n - 2;
	if (right_idx < 0)
		right_idx += philo->n;
	if (philo->fork_left && philo->fork_right)
	{	// odd philosophers take the left fork first, then the right one
		if (philo->n % 2)
		{
			pthread_mutex_lock(philo->sim->m_fork[left_idx]);
			*(philo->fork_left) = 0;
			printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
			if (philo->fork_right)
			{
				pthread_mutex_lock(philo->sim->m_fork[right_idx]);
				*(philo->fork_right) = 0;
				printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
				philo->activity = EATING;
				eat_time = timestamp(philo->sim);
				philo->last_meal = eat_time;
				printf("%09d %d is eating.\n", eat_time, philo->n);
				usleep(philo->sim->time_eat * 1000);
				*(philo->fork_right) = 1;
				pthread_mutex_unlock(philo->sim->m_fork[right_idx]);
			}
			*(philo->fork_left) = 1;
			pthread_mutex_unlock(philo->sim->m_fork[left_idx]);
			philo->activity = SLEEPING;
			printf("%09d %d is sleeping.\n", timestamp(philo->sim), philo->n);
			usleep(philo->sim->time_sleep * 100);
			philo->activity = THINKING;
		}
		else
		{
			pthread_mutex_lock(philo->sim->m_fork[right_idx]);
			*(philo->fork_right) = 0;
			printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
			if (philo->fork_left)
			{
				pthread_mutex_lock(philo->sim->m_fork[left_idx]);
				*(philo->fork_left) = 0;
				printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
				philo->activity = EATING;
				eat_time = timestamp(philo->sim);
				philo->last_meal = eat_time;
				printf("%09d %d is eating.\n", eat_time, philo->n);
				usleep(philo->sim->time_eat * 1000);
				*(philo->fork_right) = 1;
				pthread_mutex_unlock(philo->sim->m_fork[left_idx]);
			}
			*(philo->fork_left) = 1;
			pthread_mutex_unlock(philo->sim->m_fork[right_idx]);
			philo->activity = SLEEPING;
			printf("%09d %d is sleeping.\n", timestamp(philo->sim), philo->n);
			usleep(philo->sim->time_sleep * 100);
			philo->activity = THINKING;
		}
	}
}

void	*eat_sleep_die(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (!(philo->sim->sim_has_started))
		usleep(10);
	while (philo)
	{	
		if (philo->activity == THINKING
			&& philo->time_to_die < (timestamp(philo->sim) - (2 * philo->last_meal)))
			eat(philo);
		// else if (philo->activity == EATING)
		// 	sleep();
		// else if (philo->activity == SLEEPING)
		// 	think();
			//announce_activity((t_philo *) philo);
	}
	return (NULL);
}
