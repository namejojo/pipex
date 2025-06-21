/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 06:38:48 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/21 12:54:16 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../my_libft/libft.h"

void	rdwr_frm_int_fd(char *cmd_path_inc, char **env, int rd, int wr)
{
	char	**cmd;
	int		value;
	int		ind;
	char	*path;

	dup2(rd, 0);
	dup2(wr, 1);
	env = ft_split(ft_strnmat(env, "PATH=", 5) + 5, ':');
	cmd = pipex_split(cmd_path_inc, NULL, 0, 0);
	cmd_path_inc = ft_strjoin("/", *cmd);
	value = 1;
	ind = -1;
	while (env[++ind] && value)
	{
		path = ft_strjoin(env[ind], cmd_path_inc);
		value = execve(path, cmd, env);
		free(path);
	}
	ft_free_matrix(env);
	ft_free_matrix(cmd);
	free(cmd_path_inc);
	exit(0);
}

int	pipe_into_pipe(char *av, char **env, int *fd)
{
	int	fd2[2];
	int	id;

	if (pipe(fd2))
		return (perror(strerror(errno)), exit(errno), 0);
	id = fork();
	if (id < 0)
		return (perror(strerror(errno)), exit(errno), 0);
	close (fd[1]);
	if (id == 0)
		rdwr_frm_int_fd(av, env, fd[0], fd2[1]);
	close (fd[0]);
	dup2 (fd2[0], fd[0]);
	dup2 (fd2[1], fd[1]);
	close (fd2[0]);
	close (fd2[1]);
	return (id);
}
