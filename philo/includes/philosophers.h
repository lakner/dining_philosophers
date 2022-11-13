/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 20:27:36 by slakner           #+#    #+#             */
/*   Updated: 2022/11/13 17:47:41 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

# define EATING		1
# define SLEEPING 	2
# define THINKING	3

typedef struct s_philo{
	int	n;
	int	time_to_die;
	int	ate_n_times;
	int	fork_left;
	int	fork_right;
	int	activity;
}	t_philo;

typedef struct s_simulation{
	t_philo	*philos;
	int		*forks;
	int		number_philos;
	int		time_die;
	int		time_eat;
	int		time_sleep;
	int		must_eat_times;
}	t_simulation;

void			print_usage(void);
t_simulation	*read_args(int argc, char **argv);
int				validate_arg(char *arg);
int				ft_atoi(const char *str);
int				is_whitespace(const char *c);
int				sim_table(t_simulation *sim);
t_simulation	*init_sim(int num_ph, int time_die, int time_eat, int time_sl);
void			*free_sim(t_simulation *sim);
void			*eat_sleep_die(void *philo);
#endif