/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/07 17:25:14 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "my_libft/libft.h"

// maybe add??

char	*get_file(int fd)
{
	char	*str;
	char	*temp;

	temp = get_next_line(fd);
	str = NULL;
	while (temp)
	{
		str = ft_strjoin_free(str, temp, 3);
		temp = get_next_line(fd);
	}
	return (str);
}

int main(int ac, char **av, char **env)
{
	char	*str;
	int		sfd;
	int		fd[2];
	int		fd2[2];

	sfd = open(av[1], O_RDONLY);
	if (sfd < 0)
		return (perror(strerror(errno)), errno);
	str = get_file(sfd);
	close (sfd);
	if (pipe(fd))
		return (perror(strerror(errno)), errno);
	ft_putstr_fd(str, fd[1]);
	if (pipe(fd2))
		return (perror(strerror(errno)), errno);
	rd_frm_pipe_wr_int_pipe(fd[0], fd2[1]);
	sfd = open(av[ac - 1], O_WRONLY);
	if (sfd < 0)
		return (perror(strerror(errno)), errno);
	rd_frm_pipe_wr_int_pipe(fd2[0], sfd);
	close (sfd);
	free (str);
}
