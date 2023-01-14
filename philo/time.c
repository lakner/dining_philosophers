/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:40:34 by slakner           #+#    #+#             */
/*   Updated: 2023/01/14 20:27:47 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		usleep(50);
}
