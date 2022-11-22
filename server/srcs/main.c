/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeanne <ajeanne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 11:36:19 by ajeanne           #+#    #+#             */
/*   Updated: 2022/11/22 16:25:23 by ajeanne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct	s_dest
{
	char	*str;
	char	c;
	int		i;
	int		*byte;
}				t_dest;

t_dest	*dest;

int	ft_strlen(const char *str)
{
	char	*tmp;

	tmp = (char *)str;
	while (*tmp)
		tmp++;
	return (tmp - str);
}

void	modify_str()
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp = dest->str;
	if (dest->str)
		dest->str = malloc(sizeof(char) * (ft_strlen(dest->str) + 2));
	else
		dest->str = malloc(sizeof(char) * 2);
	while (tmp && tmp[i])
	{
		dest->str[i] = tmp[i];
		i++;
	}
	(dest->str)[i] = dest->c;
	(dest->str)[i + 1] = '\0';
	return ;
}

void	modify_c()
{
	int	multiplier;
	int	cnt;

	multiplier = 1;
	cnt = 0;
	while (cnt <= dest->i)
	{
		if ((dest->byte)[cnt])
			dest->c = dest->c + multiplier;
		multiplier * 2;
		cnt++;
	}
	modify_str();
	dest->i = 0;
	return ;
}

void	handler(int sig, siginfo_t *sinfo, void *oat)
{
	if (sig == SIGUSR2)
	{
		(dest->byte)[dest->i] = 1;
		kill(sinfo->si_pid, SIGUSR1);
	}
	if (sig == SIGUSR1)
	{
		(dest->byte)[dest->i] = 0;
		kill(sinfo->si_pid, SIGUSR1);
	}
	if (dest->i == 7)
		modify_c();
	if (dest->i == 7 && !(dest->c))
		exit(0);
	dest->i++;
}

int	main()
{
	pid_t	pid;
	struct	sigaction sa;

	pid = getpid();
	printf("%d\n", pid);
	dest = malloc(sizeof(t_dest));
	dest->str = NULL;
	dest->c = 0;
	dest->i = 0;
	dest->byte = malloc(sizeof(int) * 8);
	sa.sa_sigaction = &handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while(1)
	{
		pause();
	}
	return (0);
}
