/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 08:22:04 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/21 00:09:14 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../my_libft/libft.h"

int	check_one_cmd(char *str, char **env, char **cmd, int value)
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

	if (ft_strncmp(av[0], "here_doc", 9) && (access(av[1], R_OK)))
		perror(av[1]);
	ind = 1 + (access(av[1], R_OK) != 0);
	while (av[++ind + 1 - (access(av[ac - 1], W_OK))])
	{
		if (ft_strchr(av[ind], '/') || *env == NULL)
			value = access(av[ind], X_OK);
		else
			value = check_one_cmd(av[ind], env, NULL, 1);
		if (value == 1)
			return (perror(""), exit(errno));
		else if (ft_strchr(av[ind], '/') && value == -1)
			perror(av[ind]);
		else if (value == -1)
		{
			ft_putstr_fd(av[ind], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
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
		perror((*av)[*ac - 1]);
	return (fd);
}
