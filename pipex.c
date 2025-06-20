/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/20 23:57:00 by jlima-so         ###   ########.fr       */
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

	fd = open(file, O_RDONLY);
	str = get_next_line(fd);
	while (str)
	{
		ft_putstr_fd(str, wr);
		free (str);
		str = get_next_line(fd);
	}
}


int	feed_file_into_pipe(int *fd, int ac, char **av, char **ev)
{
	int		id;
	char	*str;
	
	str == NULL;
	id = fork();
	if (id < 0)
		return (perror(strerror(errno)), exit(errno), 0);
	if (id == 0)
	{
		close(fd[0]);
		if (ft_strncmp(av[0], "here_doc", 9))
			get_file(av[1], fd[1]);
		else
		{
			str = get_here_doc(av[1]);
			ft_putstr_fd(str, fd[1]);
			free(str);
		}
		close(fd[1]);
		exit(0);
	}
	return (ft_strncmp(av[0], "here_doc", 9) && access(av[1], R_OK));
}

int feed_pipe_into_file(int *fd, int ac, char **av, char **ev)
{
	char	**cmd;
	int		value;
	int		ind;
	char	*path;
	
	rdwr_frm_int_fd(av[ac - 2], ev, fd[0], fd[1]);
}

int	main(int ac, char **av, char **ev)
{
	int	fd_wr;
	int	fd[2];
	int	id;
	int	ind;

	fd_wr = check_input(&ac, &av, ev);
	if (pipe(fd) < 0)
		return (perror(strerror(errno)), errno);
	ind = 1 + feed_file_into_pipe(fd, ac, av, ev);
	while (++ind < ac - 2)
		id += pipe_into_pipe(av[ind], ev, fd);
	close(fd[1]);
	if (check_one_cmd(av[ac - 2], ev, NULL, 1) == 0 && fd_wr > 0)
	{
		id = fork();
		if (id < 0)
			return (perror(strerror(errno)), exit(errno), 0);
		if (id == 0)
			rdwr_frm_int_fd(av[ac - 2], ev, fd[0], fd_wr);
		}
	waitpid(id, NULL, 0);
	close(fd_wr);
	close(fd[0]);
	ind = (check_one_cmd(av[ac - 2], ev, NULL, 1) != 0) * 127;
	return(ind * (fd_wr > 0) + (fd_wr < 0));
}
