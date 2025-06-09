/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:08:56 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/09 19:17:24 by jlima-so         ###   ########.fr       */
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

int main (int ac, char **argv, char **env)
{
	char *cmd[] = {"awk", "{printf $1}", NULL};
	char *in_cmd = "awk {printf $1}";
	char *str = get_file(open("infile", O_RDWR));
	int id;
	int fd[2];
	char **split;

	// printf("%d\n", wrd_count(in_cmd));
	ft_print_matrix	(pipex_split(in_cmd, NULL, 0, 0));
	// printf ("\nwere done checking\n");
	// split = pipex_split(in_cmd, NULL);
	// ft_print_matrix();
	// printf("%s\n", str);
	// pipe(fd);
	// // ft_putstr_fd(str, fd[1]);
	// // free (str);
	// // dup2(fd[0], 0);
	// // close(fd[1]);
	// ft_print_matrix(cmd);
	// id = fork();
	// if (id == 0)
		// execve("/bin/awk", cmd, env);
	// close(fd[0]);
	// waitpid(id, NULL, 0);
}
