/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/09 20:13:00 by jlima-so         ###   ########.fr       */
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

// maybe add get_file??

void exit_w_msg(int number, int id)
{
	if (number)
	{
		if (id == 0)
			perror(strerror(errno));
		exit(0);
	}
}

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


char	*get_here_doc(char *here_doc)
{
	char	*ret;
	char	*str;
	char	*temp;
	char	*doc_inic;
	int		ind;

	temp = NULL;
	str = NULL;
	while (ft_strstr(str, here_doc) == NULL)
		str = ft_strjoin_free(str, temp = get_next_line(0), 3);
	doc_inic = ft_strstr(str, here_doc);
	ind = doc_inic - str;
	ret = ft_calloc (ind + 1, 1);
	while (ind--)
		ret[ind] = str[ind];
	return (ret);
}

// maybe add get_file??

void	rdwr_frm_int_fd(char *av, char **env, int *rd, int wr)
{
	t_exec	ret;
	char	**mat;
	char	*str;
	int		ind;

	dup2 (rd[0], STDIN_FILENO);
	close (rd[0]);
	dup2 (wr, STDOUT_FILENO);
	close (wr);
	ind = -1;
	ret.cmd = ft_split(av, ' ');
	str = ft_strjoin("/", ret.cmd[0]);
	mat = ft_split(ft_strmat(env, "PATH=") + 5, ':');
	while (mat[++ind])
	{
		ret.path = ft_strjoin(mat[ind], str);
		execve(ret.path, ret.cmd, env);
		free (ret.path);
	}
	ft_free_matrix(mat);
	ft_free_matrix(ret.cmd);
	free (str);
	exit (0);
}

int	feed_file_into_pipe(char **av, char **env, int *fd2)
{
	int		fd;
	int		fd1[2];
	char	*str;
	int		id;

	if (ft_strncmp(av[0], "here_doc", 9))
	{
		fd = open(av[0], O_RDONLY);
		exit_w_msg(fd < 0, 0);
		str = get_file(fd);
		close (fd);
	}
	else
		str = get_here_doc(av[1]);
	exit_w_msg(errno, 0);
	pipe(fd1);
	ft_putstr_fd(str, fd1[1]);
	free (str);
	exit_w_msg(errno, 0);
	close (fd1[1]);
	id = fork();
	exit_w_msg(id < 0, id);
	fflush(stdout);
	if (id == 0)
		rdwr_frm_int_fd (av[2], env, fd1, fd2[1]);
	close (fd1[0]);
	waitpid(id, NULL, 0);
	return (0);
}

int	pipe_into_pipe (char *av, char **env, int *fd2)
{
	int	fd1[2];
	int	id;

	if (pipe(fd1))
		return (perror(strerror(errno)), errno);
	close (fd2[1]);
	id = fork();
	if (id < 0)
		return (perror(strerror(errno)), errno);
	if (id == 0)
		rdwr_frm_int_fd(av, env, fd2, fd1[1]);
	close (fd2[0]);
	waitpid (id, NULL, 0);
	dup2 (fd1[0], fd2[0]);
	dup2 (fd1[1], fd2[1]);
	close (fd1[0]);
	close (fd1[1]);
}

int main(int ac, char **av, char **env)
{
	char	*str;
	int		fd;
	int		fd2[2];
	int		id;
	int		ind;

	if (ft_strncmp(av[1], "here_doc", 9) == 0)
	{
		av++;
		ac--;
	}
	if (ac < 5)
		return (write (2, "invalid number of argumants\n", 29));
	feed_file_into_pipe(av, env, fd2);
	fflush(stdout);
	ind = 2;
	while (ac - ind++ > 3)
		if (pipe_into_pipe (av[ind], env, fd2))
			return (exit_w_msg(errno, 0), errno);
	id = fork ();
	exit_w_msg(errno, id);
	close (fd2[1]);
	fd = open (av[ac - 1], O_WRONLY);
	exit_w_msg(errno, id);
	if (id == 0)
		rdwr_frm_int_fd (av[ac - 2], env, fd2, fd);
	close (fd2[0]);
	close (fd);
	waitpid (id, NULL, 0);
	return (0);
}
