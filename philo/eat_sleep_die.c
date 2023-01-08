/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_die.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:13:29 by slakner           #+#    #+#             */
/*   Updated: 2023/01/08 19:12:42 by slakner          ###   ########.fr       */
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

// void	announce_activity(t_philo *philo)
// {
// 	while (philo->sim->curr != philo->n || !philo->sim->sim_has_started)
// 		usleep(1);
// 	pthread_mutex_lock(&(philo->sim->m_curr));
// 	if (philo->n == 500)
// 		printf("%09d %d is thinking.\n", timestamp(philo->sim), philo->n);
// 	philo->sim->curr = (philo->sim->curr % philo->sim->num_philos) + 1; 
// 	pthread_mutex_unlock(&(philo->sim->m_curr));
// }

// void	update_time_since_last_meal(t_philo *philo)
// {
	
// }


void	unlock_all_the_mutex(t_sim *sim)
{
	int	n;

	n = 0;
	pthread_mutex_unlock(&(sim->m_dead));
	while (n < sim->num_philos - 1)
	{
		pthread_mutex_unlock(sim->m_fork[n]);
		n ++;
	}
}

int	die(t_philo *philo, int time_wait)
{
	usleep(time_wait * 1000);
	philo->dead = 1;
	pthread_mutex_lock(&(philo->sim->m_dead));
	if (philo->sim->philo_dead)
	{
		printf("Someone else dead already.");
		return (1);
	}
	philo->sim->philo_dead = 1;
	printf("%09d %d died.\n", timestamp(philo->sim), philo->n);
	unlock_all_the_mutex(philo->sim);
	return (0);
}

int	kick_the_bucket(t_philo *philo, int time_wait)
{
	if (philo->sim->philo_dead)
		return (1);
	if ((timestamp(philo->sim) + time_wait) > (philo->last_meal + philo->time_to_die))
	{
		die(philo, (philo->last_meal + philo->time_to_die) - timestamp(philo->sim));
		return (1);
	}
	return (0);
}

// int	hungry(t_philo *philo)
// {
// 	if (philo->time_to_die < (philo->last_meal * 3 / 2))
// 		return (1);
// 	return (0);
// q}

int	think(t_philo *philo)
{
	if (philo->sim->philo_dead)
		return (1);
	if (philo->activity != THINKING)
	{
		printf("%09d %d is thinking.\n", timestamp(philo->sim), philo->n);
		philo->activity = THINKING;
	}
	while ((!*(philo->fork_left) || !*(philo->fork_right)))
	{
		if (kick_the_bucket(philo, 0))
			return (1);
		usleep(500);
	}
	return (0);
}

int	grab_fork(t_philo *philo, int idx)
{
	pthread_mutex_lock(philo->sim->m_fork[idx]);
	if (kick_the_bucket(philo, 0))
	{
		pthread_mutex_unlock(philo->sim->m_fork[idx]);
		return (1);
	}
	printf("%09d %d has taken a fork.\n", timestamp(philo->sim), philo->n);
	philo->sim->fork[idx] = 0;
	return (0);
}

int	stuff_face(t_philo *philo)
{
	int		eat_time;
	int		time_to_eat;

	if (kick_the_bucket(philo, 0))
		return (1);
	philo->activity = EATING;
	eat_time = timestamp(philo->sim);
	time_to_eat = philo->sim->time_eat;
	philo->last_meal = eat_time;
	printf("%09d %d is eating.\n", eat_time, philo->n);
	while (time_to_eat > 0)
	{
		if (philo->sim->philo_dead)
			return (1);
		usleep(5000);
		time_to_eat -= 5;
	}
	usleep(time_to_eat * 1000);
	philo->ate_n_times++;
	return (0);
}

int	return_fork(t_philo *philo, int idx)
{
	if (kick_the_bucket(philo, 0))
		return (1);
	philo->sim->fork[idx] = 1;
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
			|| stuff_face(philo) || return_fork(philo, second)
			|| return_fork(philo, first))
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
	
	if (philo->sim->philo_dead)
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

void	nap(t_philo *philo)
{
	int	time_sleep;

	if (!kick_the_bucket(philo, 0))
		return ;
	time_sleep = philo->sim->time_sleep;
	if (philo->activity == EATING)
	{
		philo->activity = SLEEPING;
		if (kick_the_bucket(philo, philo->sim->time_sleep))
			return ;
		printf("%09d %d is sleeping.\n", timestamp(philo->sim), philo->n);
		while (time_sleep > 0 && !philo->sim->philo_dead)
		{
			usleep(5000);
			time_sleep -= 5;
		}
		//philo->activity = THINKING;
	}
}

void	feast(t_philo *philo)
{
	if (!kick_the_bucket(philo, 0))
		think(philo);
	if (!kick_the_bucket(philo, 0))
		eat(philo);
	if (!kick_the_bucket(philo, 0))
		nap(philo);
	return;
}

void	*eat_sleep_die(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (!(philo->sim->sim_has_started))
		usleep(10);
	// if (philo->n % 2 == 0)
	// 	usleep(50);
	while (philo && !kick_the_bucket(philo, 0))
	{	
		// if (kick_the_bucket(philo, 0))
		// 	return (NULL);
		// else
			feast(philo);
	}
	
	return (NULL);
}
