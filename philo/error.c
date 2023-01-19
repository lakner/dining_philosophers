/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slakner <slakner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 17:26:11 by slakner           #+#    #+#             */
/*   Updated: 2023/01/19 19:59:27 by slakner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	error_exit(t_sim *sim, int code, char *errstr)
{
	free_sim(sim);
	printf("Error: %s\n", errstr);
	exit (code);
}
