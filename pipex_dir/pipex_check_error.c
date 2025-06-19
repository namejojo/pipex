/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:49:04 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/19 14:00:50 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../my_libft/libft.h"

void	cant_access(char *av)
{
	int	fd;

	fd = open (av, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd >= 0)
	{
		close (fd);
		unlink(av);
		write(2, "No such file or directory: ", 28);
		write(2, av, ft_strlen(av));
		write(2, "\n", 1);
		return ;
	}
	return (permission_denied(av));
}

void	permission_denied(char *av)
{
	write (2, "permission denied: ", 20);
	write(2, av, ft_strlen(av));
	write(2, "\n", 1);
}

void	not_rdwr_frm_int_fd(int *rd, int wr)
{
	fflush(stdout);
	dup2 (rd[0], STDIN_FILENO);
	close (rd[0]);
	dup2 (wr, STDOUT_FILENO);
	close (wr);
	close (rd[1]);
	close (wr);
	exit (0);
}

int	end(t_main data, int ac, char **av, char **env)
{
	close (data.fd2[0]);
	close (data.fd);
	while (--data.ind)
		wait (NULL);
	return (check_cmd_access(av[ac - 2], env) * 127);
}
