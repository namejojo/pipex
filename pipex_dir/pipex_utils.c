/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:27:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/18 16:51:56 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../my_libft/libft.h"

void	rd_wr_didnt_work(char *str)
{
	write (2, "command not found: ", 19);
	write (2, str, ft_strlen(str));
	write (2, "\n", 1);
}

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

char	*get_file_func(char *av)
{
	int		fd;
	char	*str;

	fd = open(av, O_RDONLY, O_TRUNC, O_CREAT, 0644);
	str = get_file(fd);
	close (fd);
	return (str);
}

int	prep_here_doc(int *ac, char ***av)
{
	int	fd;

	(*av)++;
	(*ac)--;
	fd = open ((*av)[*ac - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
	return (fd);
}
 