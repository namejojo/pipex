/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/12 13:20:12 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_libft/libft.h"

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
	free (str);
	exit (0);
}

int	feed_file_into_pipe(char **av, char **env, int *fd2)
{
	int		fd;
	int		fd1[2];
	char	*str;
	int		id;

	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		return (perror(strerror(errno)), errno);
	str = get_file(fd);
	close (fd);
	if (pipe(fd1))
		return (perror(strerror(errno)), errno);
	ft_putstr_fd(str, fd1[1]);
	free (str);
	close (fd1[1]);
	if (pipe(fd2))
		return (perror(strerror(errno)), errno);
	id = fork();
	if (id < 0)
		return (perror(strerror(errno)), errno);
	if (id == 0)
		rdwr_frm_int_fd (av[2], env, fd1, fd2[1]);
	close (fd1[0]);
	waitpid(id, NULL, 0);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int		fd;
	int		fd2[2];
	int		id;

	if (ac < 5)
		return (write (2, "invalid number of argumants\n", 29));
	if (feed_file_into_pipe(av, env, fd2))
		return (errno);
	fd = open (av[4], O_WRONLY);
	id = fork ();
	close (fd2[1]);
	if (id == 0)
		rdwr_frm_int_fd (av[3], env, fd2, fd);
	close (fd2[0]);
	waitpid (id, NULL, 0);
	return (0);
}
