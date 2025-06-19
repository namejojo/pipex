/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/19 11:42:37 by jlima-so         ###   ########.fr       */
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
		perror(strerror(errno));
	ft_putstr_fd(str, fd1[1]);
	close (fd1[1]);
	free (str);
	if (pipe(fd2))
		perror(strerror(errno));
	id = fork();
	if (id == 0)
	{
		if (access(av[1], R_OK) && ft_strncmp(av[0], "here_doc", 8))
			exit (0);
		rdwr_frm_int_fd (av[2], env, fd1, fd2[1]);
	}
	close (fd1[0]);
	return (0);
}

int	pipe_into_pipe(char *av, char **env, int *fd2)
{
	int	fd1[2];
	int	id;

	if (pipe(fd1))
		perror(strerror(errno));
	close (fd2[1]);
	id = fork();
	if (id < 0)
		perror(strerror(errno));
	if (id == 0)
		rdwr_frm_int_fd(av, env, fd2, fd1[1]);
	close (fd2[0]);
	dup2 (fd1[0], fd2[0]);
	dup2 (fd1[1], fd2[1]);
	close (fd1[0]);
	close (fd1[1]);
	return (1);
}

int	last_cmd_access_else(t_exec ret, char **env)
{
	ret.mat = ft_split(ft_strmat(env, "PATH=") + 5, ':');
	while (ret.mat[++ret.ind] && ret.check == -1)
	{
		ret.path = ft_strjoin(ret.mat[ret.ind], ret.str);
		ret.check = access(ret.path, X_OK);
		free (ret.path);
	}
	ft_free_matrix(ret.mat);
	return (ret.check);
}

int	check_cmd_access(char *av, char **env)
{
	t_exec	ret;

	ret.cmd = pipex_split(av, NULL, 0, 0);
	if (ret.cmd[0][0] == '\0')
		return (ft_free_matrix(ret.cmd), 1);
	ret.str = ft_strjoin("/", ret.cmd[0]);
	ret.check = -1;
	ret.ind = -1;
	if (*env == NULL || ret.cmd[0][0] == '/')
		ret.check = access(ret.cmd[0], X_OK);
	else
		ret.check = last_cmd_access_else (ret, env);
	if (ret.check == -1)
		return (ft_free_matrix(ret.cmd), free (ret.str), 1);
	free (ret.str);
	ft_free_matrix(ret.cmd);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int	fd;
	int	fd2[2];
	int	id;
	int	ind;

	if (ac < 5 + (ft_strncmp(av[1], "here_doc", 9) == 0))
		return (write (2, "invalid number of arguments\n", 29));
	if (ft_strncmp(av[1], "here_doc", 9))
		fd = open (av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		fd = prep_here_doc(&ac, &av);
	feed_file_into_pipe(av, env, fd2);
	check_access(av + 1 + (ft_strncmp(av[1], "here_doc", 9) == 0), env);
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
		wait (NULL);
	return (check_cmd_access(av[ac - 2], env) * errno);
}
// find_cmd(av[ac - 1])
