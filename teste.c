/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:44:57 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/21 15:46:03 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_libft/libft.h"
int	main(void)
{
	int		fd;
	char	*str;

	str = get_next_line(fd);
	ft_putstr_fd("str:", 2);
	ft_putstr_fd(str, 2);
	while (str)
	{
		free (str);
		str = get_next_line(wr);
		ft_putstr_fd("str:", 2);
		ft_putstr_fd(str, 2);
	}
}
