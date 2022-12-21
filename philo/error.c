/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 17:26:11 by slakner           #+#    #+#             */
/*   Updated: 2022/12/21 18:12:33 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	error_exit(t_simulation *sim, int code, char *errstr)
{
	free_simulation(sim);
	printf("Error: %s\n", errstr);
	system("leaks philo");
	exit (code);
}
