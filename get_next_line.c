/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/15 18:49:23 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft/includes/libft.h"
#include <stdlib.h>
#include <unistd.h>

int	get_next_line(const int fd, char **line)
{
	static char	*buf;
	ssize_t		bytes;
	char		*end_of_line;
	char		*temp;
	char		first;
	size_t		len;

	*line = ft_strnew(0);
	end_of_line = NULL;
	first = 0;
	bytes = 0;
	if (!buf)
	{
		first = 1;
		buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
		if (!buf)
			return (-1);
		*buf = '\0';
	}
	else
		end_of_line = ft_strchr(buf, '\n');
	len = ft_strlen(buf);
	if (!end_of_line && len)
	{
		buf += len - BUFF_SIZE;
		ft_strdel(&(buf));
	}
	while (!end_of_line)
	{
		if (buf)
		{
			temp = *line;
			*line = ft_strjoin(*line, buf);
			free(temp);
			if (buf && bytes != BUFF_SIZE && !first)
				buf = ft_strchr(buf, '\0') - BUFF_SIZE;
		}
		bytes = read(fd, buf, BUFF_SIZE);
		if (bytes < 0)
			return ((int)bytes);
		if (!bytes)
			return (0);
		buf[bytes] = '\0';
		end_of_line = ft_strchr(buf, '\n');
		if (first && bytes < BUFF_SIZE) //branchless?
			*((char *)ft_memset(&buf[bytes + !end_of_line], -1, \
				(size_t)(BUFF_SIZE - bytes)) + (BUFF_SIZE - bytes)) = '\0';
		if (!end_of_line && bytes < BUFF_SIZE)
		{
			temp = *line;
			*line = ft_strjoin(*line, buf);
			free(temp);
			buf[BUFF_SIZE] = '\0';
			buf += BUFF_SIZE;
			return (1);
		}
	}
	*end_of_line = '\0';
	temp = *line;
	*line = ft_strjoin(*line, buf);
	free(temp);
	*end_of_line = '\n';
	buf = end_of_line + 1; //overrun?
	return (1);
}
