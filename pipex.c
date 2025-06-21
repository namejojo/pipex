/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/21 23:53:47 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_libft/libft.h"

char	*get_here_doc(char *limiter)
{
	char	*str;
	char	*temp;

	str = NULL;
	temp = get_next_line(0);
	while (ft_strncmp(temp, limiter, ft_strlen(temp) - 1))
	{
		str = ft_strjoin_free(str, temp, 3);
		temp = get_next_line(0);
	}
	free(temp);
	return (str);
}

void	get_file(char *file, int wr)
{
	int		fd;
	char	*str;

	fd = open(file, O_RDONLY, 0644);
	if (fd > 0)
	{
		str = get_next_line(fd);
		while (str)
		{
			ft_putstr_fd(str, wr);
			free (str);
			str = get_next_line(fd);
		}
		close(fd);
	}
}

int	feed_file_into_pipe(int *fd, char **av, int to_close)
{
	int		id;
	char	*str;

	str = NULL;
	id = fork();
	if (id < 0)
		return (perror(strerror(errno)), exit(errno), 0);
	if (id == 0)
	{
		if (to_close > 0)
			close(to_close);
		if (ft_strncmp(av[0], "here_doc", 9))
			get_file(av[1], fd[1]);
		else
		{
			str = get_here_doc(av[1]);
			ft_putstr_fd(str, fd[1]);
			free(str);
		}
		close(fd[0]);
		close(fd[1]);
		exit(0);
	}
	return (ft_strncmp(av[0], "here_doc", 9) && access(av[1], R_OK));
}

int	main(int ac, char **av, char **ev)
{
	int	fd[3];
	int	id;
	int	ind;

	fd[2] = check_input(&ac, &av, ev);
	if (pipe(fd) < 0)
		return (perror(strerror(errno)), close(fd[2]), errno);
	ind = 1 + feed_file_into_pipe(fd, av, fd[2]);
	while (++ind < ac - 2)
		id = pipe_into_pipe(av[ind], ev, fd, fd[2]);
	close(fd[1]);
	if ((check_one_cmd(av[ac - 2], ev, NULL, 1) == 0) && fd[2] > 0)
	{
		id = fork();
		if (id < 0)
			return (perror(strerror(errno)), exit(errno), 0);
		if (id == 0)
			rdwr_frm_int_fd(av[ac - 2], ev, fd[0], fd[2]);
	}
	waitpid(id + (id < 0), NULL, 0);
	if (fd[2] > 0)
		close(fd[2]);
	close(fd[0]);
	ind = (check_one_cmd(av[ac - 2], ev, NULL, 1)) * 127;
	return (ind * (fd[2] > 0) + (fd[2] < 0));
}
