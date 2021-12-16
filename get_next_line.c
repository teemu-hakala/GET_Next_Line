/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/16 21:20:48 by thakala          ###   ########.fr       */
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

	if (fd < 0 || !line)
		return (-1);
	*line = ft_strnew(0);
	end_of_line = NULL;
	if (!buf)
	{
		buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
		if (!buf)
			return (-1);
		*buf = '\0';
		*(buf + BUFF_SIZE) = '\0';
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
			if (ft_strlen(*line))
			{
				*buf = '\0';
				return (1);
			}
			free(buf);
			buf = NULL;
			return (0); // bytes <= 0; return (bytes);
		}
		buf[bytes] = '\0';
		end_of_line = ft_strchr(buf, '\n');
	}
	*end_of_line = '\0';
	temp = *line;
	*line = ft_strjoin(*line, buf);
	free(temp);
	temp = buf;
	buf = ft_strdup(end_of_line + 1);
	free(temp);
	if (!buf)
		return (-1);
	return (1);
}
