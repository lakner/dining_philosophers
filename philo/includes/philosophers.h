/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 20:27:36 by slakner           #+#    #+#             */
/*   Updated: 2023/01/14 18:42:43 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

# define EATING		1
# define SLEEPING 	2
# define THINKING	3

struct	s_simulation;

typedef struct s_philo{
	int					n;
	int					time_to_die;
	int					ate_n_times;
	int					just_ate;
	int					*fork_left;
	int					*fork_right;
	int					has_fork_idx1;
	int					has_fork_idx2;
	int					activity;
	int					last_meal;
	int					dead;
	struct s_simulation	*sim;
	pthread_t			thread;
}	t_philo;

typedef struct s_simulation{
	int				num_philos;
	t_philo			*philo;
	int				*fork;
	pthread_mutex_t	**m_fork;
	pthread_mutex_t	m_dead;
	int				philo_dead;
	pthread_mutex_t	m_full;
	int				num_philos_full;
	pthread_mutex_t	m_speak;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	int				must_eat_times;
	int				sim_has_started;
	long			time_start;

}	t_sim;

/* philosophers.c */
void	print_usage(void);

/* read_args.c */
t_sim	*read_args(int argc, char **argv);
int		validate_arg(char *arg);
int		is_whitespace(const char *c);
int		ft_atoi(const char *str);

/* init_sim.c */
t_sim	*init_sim(int num_ph, int time_die, int time_eat, int time_sl);
int		sim_table(t_sim *sim);
void	prepare_philos(t_sim *sim);
void	prepare_forks(t_sim *sim);

/* sim.c */
void	release_philos(t_sim *sim);
void	wait_for_the_end(t_sim *sim);
void	close_mutexes(t_sim *sim);
void	free_sim(t_sim *sim);

/* time.c */
long	timestamp(t_sim *sim);
void	wait_for(t_sim *sim, int micros);

/**/
void	*eat_sleep_die(void *philo);
void	say(t_philo *philo, long timestamp, char *msg);

/* eat.c */
int		hungry(t_philo *philo);
int		grab_fork(t_philo *philo, int idx);
int		stuff_face(t_philo *philo);
int		return_fork(t_philo *philo, int idx);
int		indulge_gluttony(t_philo *philo, int first, int second);

/* die.c */
int		die(t_philo *philo, int time_wait);
int		kick_the_bucket(t_philo *philo, int time_wait);

/* error.c */
void	error_exit(t_sim *sim, int code, char *errstr);


#endif