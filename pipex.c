/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:19:15 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/20 08:00:03 by jlima-so         ###   ########.fr       */
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

void	check_rd_file(char *file)
{
	if (access(file, R_OK))
	{
		if (close(open(file, O_RDONLY | O_APPEND | O_CREAT, 0644)))
		{
			ft_putstr_fd("permission denied: ", 2);
			ft_putendl_fd(file, 2);
		}
		else
		{
			unlink(file);
			ft_putstr_fd("no such file or directory: ", 2);
			ft_putendl_fd(file, 2);
		}
	}
}

int	check_one_cmd_w_env(char *str, char **env, char **cmd, int value)
{
	char	*cmd_path;
	int		ind;

	env = ft_split(ft_strnmat(env, "PATH=", 5), ':');
	if (env == NULL)
		return (1);
	cmd = pipex_split(str, NULL, 0, 0);
	if (cmd == NULL)
		return (ft_free_matrix(env), 1);
	str = ft_strjoin("/", *cmd);
	if (str == NULL)
		return (ft_free_matrix(cmd), ft_free_matrix(env), 1);
	ind = -1;
	while (env[++ind] && value)
	{
		cmd_path = ft_strjoin (env[ind], str);
		if (cmd_path == NULL)
			return (free(str), ft_free_matrix(env), ft_free_matrix(cmd), 1);
		value = access(cmd_path, X_OK);
		free(cmd_path);
	}
	free (str);
	ft_free_matrix(cmd);
	ft_free_matrix(env);
	return (value);
}

void	check_all_cmd(int ac, char **av, char **env)
{
	int	ind;
	int	value;
	
	if (ft_strncmp(av[0], "here_doc", 9))
		check_rd_file(av[1]);
	av += (access(av[1], R_OK) != 0);
	ind = 1;
	while (av[++ind + 1])
	{
		if (av[ind][0] == '/' || *env == NULL)
			value = access(av[ind], X_OK) * 2;
		else
			value = check_one_cmd_w_env(av[ind], env, NULL, 1);
		if (value == 1)
			return (perror(strerror(errno)), exit(errno));
		else if (value == -1)
		{
			ft_putstr_fd("command not found: ", 2);
			ft_putendl_fd(av[ind], 2);
		}
		if (value == -2)
			ft_putstr_fd("no such file or directory: ", 2);
		if (value == -2)
			ft_putendl_fd(av[ind], 2);
	}
}

int	check_input(int *ac, char ***av, char **env)
{
	int	fd;

	(*ac) -= (ft_strncmp((*av)[1], "here_doc", 9) == 0);
	(*av) += (ft_strncmp((*av)[1], "here_doc", 9) == 0);
	if (*ac < 5)
	{
		ft_putendl_fd("invalid number of arguments", 2);
		exit(0);
	}
	check_all_cmd(*ac, *av, env);
	if (ft_strncmp((*av)[0], "here_doc", 9))
		fd = open((*av)[*ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		fd = open((*av)[*ac - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("permission denied: ", 2);
		ft_putendl_fd((*av)[*ac - 1], 2);
	}
	return (fd);
}

int	main(int ac, char **av, char **env)
{
	int	fd;

	fd = check_input(&ac, &av, env);
}
// find_cmd(av[ac - 1])
