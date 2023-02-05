/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie.lakner <stephanie.lakner@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 15:57:57 by slakner           #+#    #+#             */
/*   Updated: 2023/02/05 18:47:22 by stephanie.l      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_sim	*sim;

	if (argc < 5 || argc > 6)
	{
		printf("Wrong number of arguments.\n");
		print_usage();
		return (1);
	}
	sim = read_args(argc, argv);
	if (!sim)
	{
		printf("Error reading simulation parameters.\n");
		print_usage();
		return (1);
	}
	if (argc == 6 && !sim->must_eat_times)
		return (0);
	return (sim_table(sim));
}

void	print_usage(void)
{
	printf("Usage:\n");
	printf("philo <number_of_philosophers> <time_to_die> <time_to_eat> ");
	printf("<time_to_sleep> [number_of_times_each_philosopher_must_eat]");
}
