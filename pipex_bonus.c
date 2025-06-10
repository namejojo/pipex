/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/10 11:07:04 by jlima-so         ###   ########.fr       */
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
	char	*str;
	char	*temp;

	temp = get_next_line(0);
	str = NULL;
	while (ft_strncmp(here_doc, temp, ft_strlen(temp) - 1))
	{
		str = ft_strjoin_free(str, temp, 3);
		temp = get_next_line(0);
	}
	free (temp);
	return (str);
}

void	rd_wr_didnt_work(char *str)
{
	write (2, "command not found: ", 19);
	write (2, str, ft_strlen(str));
	write (2, "\n", 1);
}


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
	ret.cmd = pipex_split(av, NULL, 0, 0);
	str = ft_strjoin("/", ret.cmd[0]);
	mat = ft_split(ft_strmat(env, "PATH=") + 5, ':');
	while (mat[++ind])
	{
		ret.path = ft_strjoin(mat[ind], str);
		execve(ret.path, ret.cmd, env);
		free (ret.path);
	}
	rd_wr_didnt_work(str + 1);
	ft_free_matrix(mat);
	ft_free_matrix(ret.cmd);
	close (rd[1]);
	free (str);
	exit (errno);
}

char	*get_file_func(char *av)
{
	int		fd;
	char	*str;

	fd = open(av, O_RDONLY);
	str = get_file(fd);
	exit_w_msg(errno, 0);
	close (fd);
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
	exit_w_msg(pipe(fd1), 0);
	ft_putstr_fd(str, fd1[1]);
	free (str);
	close (fd1[1]);
	exit_w_msg(pipe(fd2), 0);
	id = fork();
	exit_w_msg(id < 0, id);
	if (id == 0)
		rdwr_frm_int_fd (av[2], env, fd1, fd2[1]);
	exit_w_msg(errno, 0);
	close (fd1[0]);
	waitpid(id, NULL, 0);
	return (0);
}

int	pipe_into_pipe (char *av, char **env, int *fd2)
{
	int	fd1[2];
	int	id;

	if (pipe(fd1))
		return (errno);
	close (fd2[1]);
	id = fork();
	if (id < 0)
		return (errno);
	if (id == 0)
		rdwr_frm_int_fd(av, env, fd2, fd1[1]);
	exit_w_msg(errno, 0);
	close (fd2[0]);
	waitpid (id, NULL, 0);
	dup2 (fd1[0], fd2[0]);
	dup2 (fd1[1], fd2[1]);
	close (fd1[0]);
	close (fd1[1]);
	return (errno);
}

void	here_doc (int *fd, int *ac, char ***av)
{
	close (*fd);
	(*av)++;
	(*ac)--;
	*fd = open ((*av)[*ac - 1], O_WRONLY | O_APPEND | O_CREAT);
	exit_w_msg(errno, 0);
}

int main(int ac, char **av, char **env)
{
	int		fd;
	int		fd2[2];
	int		id;
	int		ind;

	if (ac < 5 + (ft_strncmp(av[1], "here_doc", 9) == 0))
		return (write (2, "invalid number of argumants\n", 29));
	fd = open (av[ac - 1], O_WRONLY);
	if (ft_strncmp(av[1], "here_doc", 9) == 0)
		here_doc(&fd, &ac, &av);
	feed_file_into_pipe(av, env, fd2);
	ind = 2;
	while (ac - ind++ > 3)
		exit_w_msg(pipe_into_pipe (av[ind], env, fd2), 0);
	id = fork ();
	exit_w_msg(errno, id);
	close (fd2[1]);
	if (id == 0)
		rdwr_frm_int_fd (av[ac - 2], env, fd2, fd);
	exit_w_msg(errno, 0);
	close (fd2[0]);
	close (fd);
	waitpid (id, NULL, 0);
	return (0);
}
