/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/16 13:43:41 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft/includes/libft.h"
#include <stdlib.h>
#include <unistd.h>

int	get_next_line(const int fd, char **line)
{
	static char	*buf;
	static char	*debug;
	char		*end_of_line;
	char		*temp;
	ssize_t		bytes;

	*line = ft_strnew(0);
	end_of_line = NULL;
	if (!buf)
	{
		buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 2));
		if (!buf)
			return (-1);
		*buf = '\0';
		ft_memset(buf + 1, -3, BUFF_SIZE - 1);
		ft_memcpy(buf + BUFF_SIZE, "\0\xff", 2);
		debug = buf;
	}
	else
		end_of_line = ft_strchr(buf, '\n');
	while (!end_of_line)
	{
		temp = *line;
		*line = ft_strjoin(*line, buf);
		free(temp);
		if (!*line) //err = -!*line;
			return (-1);
		bytes = read(fd, buf, BUFF_SIZE);
		if (bytes < 0)
			return (-1); //err = bytes; (man 2 read) -> -1
		if (!bytes)
		{
			end_of_line = ft_strchr(buf, '\0');
			if (*(end_of_line + 1) == '\xfe') //existance of e_o_l guaranteed
			{
				end_of_line = ft_strchr(end_of_line + 1, '\0');
				if (*(end_of_line + 1) == '\xff')
					free(end_of_line - BUFF_SIZE);
				buf = NULL;
			}
			else if (*(end_of_line + 2) == '\xff')
			{
				free(end_of_line + 1 - BUFF_SIZE);
				buf = NULL;
			}
			return (0); // bytes <= 0; return (bytes);
		}
		else if (bytes < BUFF_SIZE)
			ft_memcpy(buf + bytes, "\0\xfe", 2 - (bytes + 1 == BUFF_SIZE));
		end_of_line = ft_strchr(buf, '\n');
	}
	*end_of_line = '\0';
	temp = *line;
	*line = ft_strjoin(*line, buf);
	free(temp);
	*end_of_line = '\xfe';
	buf = end_of_line + 1;
	return (1);
}
