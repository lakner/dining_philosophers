/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 20:27:36 by slakner           #+#    #+#             */
/*   Updated: 2022/11/12 20:41:48 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

typedef struct s_simulation{
	int	number_philosophers;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	//int	forks_on_table; //(?)
}	t_simulation;

typedef struct s_philo{
	int	n;
	int	time_to_die;
	int	ate_n_times;
	int	fork_left;
	int	fork_right;
}	t_philo;

#endif