/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:02:22 by slakner           #+#    #+#             */
/*   Updated: 2023/01/13 23:07:04 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_sim	*read_args(int argc, char **argv)
{
	int		i;
	t_sim	*sim;

	i = 1;
	while (i < argc)
	{
		if (!validate_arg(argv[i]))
			return (NULL);
		i++;
	}
	sim = init_sim(ft_atoi(argv[1]), ft_atoi(argv[2]),
			ft_atoi(argv[3]), ft_atoi(argv[4]));
	if (argc == 6)
		sim->must_eat_times = ft_atoi(argv[5]);
	else
		sim->must_eat_times = 0;
	return (sim);
}

t_sim	*init_sim(int num_ph, int time_die, int time_eat, int time_sl)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		error_exit(sim, 2, "Malloc error.");
	sim->num_philos = num_ph;
	sim->num_philos_full = 0;
	pthread_mutex_init(&(sim->m_dead), NULL);
	pthread_mutex_init(&(sim->m_full), NULL);
	sim->time_die = time_die * 1000;
	sim->time_eat = time_eat * 1000;
	sim->time_sleep = time_sl * 1000;
	sim->sim_has_started = 0;
	return (sim);
}

int	validate_arg(char *arg)
{
	while (*arg)
	{
		if (*arg < '0' || *arg > '9')
			return (0);
		arg++;
	}
	return (1);
}

int	is_whitespace(const char *c)
{
	if (*c == ' ' || *c == '\n' || *c == '\t'
		|| *c == '\v' || *c == '\f' || *c == '\r')
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int	number;
	int	digit;
	int	i;
	int	positive_negative;

	number = 0;
	positive_negative = 1;
	i = 0;
	while (is_whitespace(str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			positive_negative *= -1;
		str ++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		number = number * 10 + digit;
		i++;
	}
	return (number * positive_negative);
}