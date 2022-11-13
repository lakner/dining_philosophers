/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:37:18 by slakner           #+#    #+#             */
/*   Updated: 2022/11/13 17:48:01 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	sim_table(t_simulation *sim)
{
	int			i;
	pthread_t	th;

	i = 0;
	while (i < sim->number_philos)
	{
		sim->philos[i].n = i + 1;
		sim->philos[i].time_to_die = sim->time_die;
		sim->philos[i].ate_n_times = 0;
		sim->philos[i].fork_left = 0;
		sim->philos[i].fork_right = 0;
		sim->philos[i].activity = THINKING;
		pthread_create(&th, NULL, eat_sleep_die, &(sim->philos[i]));
		i ++;
	}
	free_sim(sim);
	return (0);
}

t_simulation	*init_sim(int num_ph, int time_die, int time_eat, int time_sl)
{
	t_simulation	*sim;

	sim = malloc(sizeof(t_simulation));
	if (!sim)
		return (NULL);
	sim->philos = malloc(num_ph * sizeof(t_philo));
	if (!sim->philos)
		return (free_sim(sim));
	sim->forks = malloc(num_ph * sizeof(int));
	if (!sim->forks)
		return (free_sim(sim));
	sim->number_philos = num_ph;
	sim->time_die = time_die;
	sim->time_eat = time_eat;
	sim->time_sleep = time_sl;
	return (sim);
}

void	*free_sim(t_simulation *sim)
{
	if (sim)
	{
		if (sim->philos)
			free(sim->philos);
		if (sim->forks)
			free(sim->forks);
		free(sim);
	}
	return (NULL);
}
