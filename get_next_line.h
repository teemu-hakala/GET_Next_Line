/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:23:04 by thakala           #+#    #+#             */
/*   Updated: 2021/12/24 23:53:34 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFF_SIZE 7
# define FD_MAX 10240
# define CLEAR 0b0
# define FETCH 0b1
# define UPDATE 0b10
# include "libft/includes/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

typedef struct s_remnant
{
	char	*str;
	size_t	len;
	char	*idx;
}	t_rem;

int	get_next_line(const int fd, char **line);

#endif
