/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 20:27:36 by slakner           #+#    #+#             */
/*   Updated: 2022/12/21 23:04:34 by slakner          ###   ########.fr       */
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

struct	s_simulation;

typedef struct s_philo{
	int					n;
	int					time_to_die;
	int					ate_n_times;
	int					fork_left;
	int					fork_right;
	int					activity;
	pthread_t			thread;
	int					dead;
	struct s_simulation	*sim;
}	t_philo;

typedef struct s_simulation{
	t_philo			*philo;
	int				*fork;
	pthread_mutex_t	*m_fork;
	pthread_mutex_t	m_curr;
	//pthread_mutex_t	m_announcing;
	int				curr;
	int				*philo_dead;
	int				num_philos;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				must_eat_times;
	struct timeval	time_start;
}	t_sim;

void	print_usage(void);
t_sim	*read_args(int argc, char **argv);
int		validate_arg(char *arg);
int		ft_atoi(const char *str);
int		is_whitespace(const char *c);
int		sim_table(t_sim *sim);
t_sim	*init_sim(int num_ph, int time_die, int time_eat, int time_sl);
void	free_sim(t_sim *sim);
void	*eat_sleep_die(void *philo);
void	error_exit(t_sim *sim, int code, char *errstr);
void	release_philos(t_sim *sim);
void	create_mutexes(t_sim *sim);
void	wait_for_the_end(t_sim *sim);

#endif