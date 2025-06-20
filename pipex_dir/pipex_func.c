/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 06:38:48 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/20 06:48:57 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../my_libft/libft.h"

int	pipe_into_pipe(char *av, char **env, int *fd2)
{
	int	fd1[2];
	int	id;

	if (pipe(fd1))
		perror(strerror(errno));
	close (fd2[1]);
	id = fork();
	if (id < 0)
		perror(strerror(errno));
	// if (id == 0)
		// rdwr_frm_int_fd(av, env, fd2, fd1[1]);
	close (fd2[0]);
	dup2 (fd1[0], fd2[0]);
	dup2 (fd1[1], fd2[1]);
	close (fd1[0]);
	close (fd1[1]);
	return (1);
}

