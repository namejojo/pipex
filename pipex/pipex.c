/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/07 14:55:52 by jlima-so         ###   ########.fr       */
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

int use_pipes(char *path, char **cmd, char **env, int *fd)
{
	dup2(fd[0], STDIN_FILENO);
	close (fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close (fd[1]);
	execve (path, cmd, env);
	exit (0);
}

int main(int ac, char **av, char **env)
{
	char	path[] = "/bin/ls";
	char	*cmd[] = {"ls", "-l", NULL};
	char	path2[] = "/bin/wc";
	char	*cmd2[] = {"wc", "-l", NULL};
	char	*str;
	int		id1;
	int		id2;
	int		n;
	int		fd[2];

	if (pipe(fd) == -1)
		return (perror(strerror(errno)), errno);
	id1 = fork();
	if (id1 == -1)
		return (perror(strerror(errno)), errno);
	if (id1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close (fd[0]);
		close (fd[1]);
		execve (path, cmd, env);
		exit (0);
	}
	waitpid (id1, NULL, 0);
	
	// str = get_next_line(fd[0]);
	// while (str)
	// {
	// 	close (fd[1]);
	// 	write (1, str, ft_strlen(str));
	// 	free(str);
	// 	str = get_next_line(fd[0]);
	// }

	id2 = fork();
	if (id2 == -1)
		return (perror(strerror(errno)), errno);
	if (id2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close (fd[0]);
		close (fd[1]);
		execve (path2, cmd2, env);
		exit (0);
	}
	waitpid (id2, NULL, 0);
}
