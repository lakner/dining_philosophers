/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 15:57:57 by slakner           #+#    #+#             */
/*   Updated: 2022/11/13 16:31:18 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (argc < 5 || argc > 6)
	{
		printf("Wrong number of arguments.\n")
		print_usage();
		return (1);
	}
	sim = read_args(argc, argv);
	return (eat_sleep_die(sim));
}

void	print_usage(void)
{
	printf("Usage:\n");
	printf("philo <number_of_philosophers> <time_to_die> <time_to_eat> ");
	printf("<time_to_sleep> [number_of_times_each_philosopher_must_eat]");
}