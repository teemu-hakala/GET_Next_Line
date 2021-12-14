/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:23:04 by thakala           #+#    #+#             */
/*   Updated: 2021/12/14 00:30:15 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFF_SIZE 32
# include <string.h>

int	get_next_line(const int fd, char **line);

typedef struct s_buffer
{
	char	*buf;
	char	*pos;
	size_t	rem;
	size_t	count;
	int		fd;
}	t_buffer;

/*
typedef struct s_buffer
{
	int		fd;
	char	*buf;
	size_t	idx;
	size_t	count;
}	t_buffer;
*/

#endif
