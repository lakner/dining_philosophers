/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:02:22 by slakner           #+#    #+#             */
/*   Updated: 2022/11/13 17:25:48 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_simulation	*read_args(int argc, char **argv)
{
	int				i;
	t_simulation	*sim;

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

int	validate_arg(char *arg)
{
	while (arg)
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