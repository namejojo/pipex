/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:47:00 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/12 12:46:47 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../my_libft/libft.h"

int	check_access_else(t_exec ret, char **env)
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

void	check_access(char **av, char **env)
{
	t_exec	ret;

	av = av + (ft_strncmp(av[0], "here_doc", 8) == 0);
	while ((++av)[1] != NULL)
	{
		ret.cmd = pipex_split(av[0], NULL, 0, 0);
		ret.str = ft_strjoin("/", ret.cmd[0]);
		ret.check = -1;
		ret.ind = -1;
		if (*env == NULL || ret.cmd[0][0] == '/')
			ret.check = access(ret.cmd[0], X_OK);
		else
			ret.check = check_access_else (ret, env);
		if (ret.check == -1)
			rd_wr_didnt_work(ret.str + 1);
		free (ret.str);
		ft_free_matrix(ret.cmd);
		if (ret.check == -1)
			find_error();
	}
}

void	rdwr_frm_int_fd_aux(t_exec ret, char **env)
{
	int		ind;
	char	*str;
	char	**mat;

	ind = -1;
	if (*env == NULL || ret.cmd[0][0] == '/')
	{
		execve(ret.cmd[0], ret.cmd, env);
		ft_free_matrix(ret.cmd);
		exit(0);
	}
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
}

void	rdwr_frm_int_fd(char *av, char **env, int *rd, int wr)
{
	t_exec	ret;

	dup2 (rd[0], STDIN_FILENO);
	close (rd[0]);
	dup2 (wr, STDOUT_FILENO);
	close (wr);
	ret.cmd = pipex_split(av, NULL, 0, 0);
	rdwr_frm_int_fd_aux(ret, env);
	close (rd[1]);
	close (wr);
	exit (0);
}

void	find_error(void)
{
	exit(1);
}
