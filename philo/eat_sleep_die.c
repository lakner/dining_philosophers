/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_die.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:13:29 by slakner           #+#    #+#             */
/*   Updated: 2023/01/14 18:47:47 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	say(t_philo *philo, long timestamp, char *msg)
{
	if (kick_the_bucket(philo, 0))
		return ;
	pthread_mutex_lock(&(philo->sim->m_dead));
	printf("%012ld %d %s\n", timestamp, philo->n, msg);
	pthread_mutex_unlock(&(philo->sim->m_dead));
}

int	think(t_philo *philo)
{
	if (kick_the_bucket(philo, 0))
		return (1);
	if (philo->activity != THINKING)
	{
		say(philo, timestamp(philo->sim)/1000, "is thinking.");
		philo->activity = THINKING;
	}
	while ((!*(philo->fork_left) || !*(philo->fork_right)) || !hungry(philo))
	{
		if (kick_the_bucket(philo, 0))
			return (1);
		wait_for(philo->sim, 1);
	}
	return (0);
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
		say(philo, timestamp(philo->sim) / 1000, "is sleeping.");
		if (kick_the_bucket(philo, philo->sim->time_sleep))
			return (1);
		wait_for(philo->sim, time_sleep);
	}
	return (0);
}

int	eat(t_philo *philo)
{
	int		left_idx;
	int		right_idx;

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

int	feast(t_philo *philo)
{
	if (think(philo))
		return (1);
	if (eat(philo))
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
