/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/12 13:20:08 by jlima-so         ###   ########.fr       */
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

char	*get_here_doc(char *here_doc)
{
	char	*str;
	char	*temp;

	temp = get_next_line(0);
	str = NULL;
	while (ft_strncmp(here_doc, temp, ft_strlen(here_doc))
		|| ft_strlen(here_doc) != ft_strlen(temp) - 1)
	{
		str = ft_strjoin_free(str, temp, 3);
		temp = get_next_line(0);
	}
	free (temp);
	return (str);
}

int	feed_file_into_pipe(char **av, char **env, int *fd2)
{
	int		fd1[2];
	char	*str;
	int		id;

	if (ft_strncmp(av[0], "here_doc", 9))
		str = get_file_func(av[1]);
	else
		str = get_here_doc(av[1]);
	if (pipe(fd1))
		exit(1);
	ft_putstr_fd(str, fd1[1]);
	close (fd1[1]);
	free (str);
	if (pipe(fd2))
		exit(1);
	id = fork();
	if (id == 0)
		rdwr_frm_int_fd (av[2], env, fd1, fd2[1]);
	close (fd1[0]);
	return (0);
}

int	pipe_into_pipe(char *av, char **env, int *fd2)
{
	int	fd1[2];
	int	id;

	if (pipe(fd1))
		exit(1);
	close (fd2[1]);
	id = fork();
	if (id < 0)
		exit (errno);
	if (id == 0)
		rdwr_frm_int_fd(av, env, fd2, fd1[1]);
	close (fd2[0]);
	dup2 (fd1[0], fd2[0]);
	dup2 (fd1[1], fd2[1]);
	close (fd1[0]);
	close (fd1[1]);
	return (1);
}

int	main(int ac, char **av, char **env)
{
	int	fd;
	int	fd2[2];
	int	id;
	int	ind;

	if (ac < 5 + (ft_strncmp(av[1], "here_doc", 9) == 0))
		return (write (2, "invalid number of argumants\n", 29));
	check_access(av + 1, env);
	if (ft_strncmp(av[1], "here_doc", 8))
		fd = open (av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		fd = prep_here_doc(&ac, &av);
	feed_file_into_pipe(av, env, fd2);
	ind = 2;
	while (ac - ind++ > 3)
		pipe_into_pipe (av[ind], env, fd2);
	close (fd2[1]);
	id = fork ();
	if (id == 0)
		rdwr_frm_int_fd (av[ac - 2], env, fd2, fd);
	close (fd2[0]);
	close (fd);
	while (--ind)
		waitpid (id, NULL, 0);
	return (0);
}
