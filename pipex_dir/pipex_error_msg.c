/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error_msg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:49:04 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/19 11:28:46 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../my_libft/libft.h"

void	cant_access(char *av)
{
	write(2, "No such file or directory: ", 28);
	write(2, av, ft_strlen(av));
	write(2, "\n", 1);
}
