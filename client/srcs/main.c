/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeanne <ajeanne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 11:36:19 by ajeanne           #+#    #+#             */
/*   Updated: 2022/11/22 17:14:16 by ajeanne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct	s_needs
{
	unsigned char	c;
	char			*str;
	int				i;
	int				pos;
}			t_needs;

t_needs	*needs;

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - 48);
		i++;
	}
	return (res * sign);
}

void	sender(int pid)
{
	needs->c = needs->str[needs->pos];
	printf("%d\n", needs->c >> needs->i & 1);
	if (needs->i < 8)
	{
		if (needs->c >> needs->i & 1 == 1)
			kill(pid, SIGUSR2);
		else if (needs->c >> needs->i & 1 == 0)
			kill(pid, SIGUSR1);
	}
	needs->i++;
	if (needs->i == 8)
	{
		needs->i = 0;
		needs->pos++;
	}
	return ;
}

void	handler(int sig, siginfo_t *info, void *oat)
{
	if (sig == SIGUSR1)
		sender(info->si_pid);
	if (sig == SIGUSR2)
		kill(getpid(), 1);
	return ;
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;

	needs = malloc(sizeof(t_needs));
	needs->str = argv[2];
	needs->i = 0;
	needs->pos = 0;
	sa.sa_sigaction = &handler;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) < 0)
		return (0);
	if (sigaction(SIGUSR2, &sa, NULL) < 0)
		return (0);
	sender(atoi(argv[1]));
	while (1)
	{
		pause();
	}
	return (0);
}
