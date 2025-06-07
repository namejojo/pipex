/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:00:17 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/06 17:49:32 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_libft/libft.h"

int main (void)
{
	char	*str;
	int		fd;
	int		fd2;
	
	fd = open("outfile", O_RDWR);
	fd2 = open("otherfile", O_RDWR);
	str = get_next_line(fd);
	dup2(fd2, 1);
	while (str)
	{
		printf("%s",str);
		free (str);
		str = get_next_line(fd);
	}
}

int		n;
int		id;
int		error;
char	*str;
char	*cmd_path;
char	**cmd_to_pipe;
int		fd[2];
char	**matrix;

if (pipe(fd) == -1)
	return (printf("An error has occurred opening pipe\n"));
id = fork();
if (id == -1)
	return (printf("fork didnt work"));
if (id != 0)
	wait (&n);
if (id == 0)
{
	function (ac, av, env, fd);
	return (0);
}
n = open ("outfile", O_RDWR);
// free (str);
str = get_next_line(fd[0]);
while (str)
{
	write(n, str, ft_strlen(str));
	free(str);
	str = get_next_line(fd[0]);
}
