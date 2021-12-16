/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/16 21:47:16 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft/includes/libft.h"
#include <stdlib.h>
#include <unistd.h>

int	get_next_line(const int fd, char **line)
{
	static char	*buf[FD_MAX + 1];
	static char	**debug;
	char		*end_of_line;
	char		*temp;
	ssize_t		bytes;

	if (fd < 0 || !line || fd > FD_MAX)
		return (-1);
	*line = ft_strnew(0);
	end_of_line = NULL;
	if (!buf[fd])
	{
		buf[fd] = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
		if (!buf[fd])
			return (-1);
		*(buf[fd]) = '\0';
		*(buf[fd] + BUFF_SIZE) = '\0';
		debug = buf;
	}
	else
		end_of_line = ft_strchr(buf[fd], '\n');
	while (!end_of_line)
	{
		temp = *line;
		*line = ft_strjoin(*line, buf[fd]);
		free(temp);
		if (!*line) //err = -!*line;
			return (-1);
		temp = buf[fd];
		buf[fd] = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
		if (!buf[fd])
			return (-1);
		//buf[fd][BUFF_SIZE] = '\0';
		free(temp);
		bytes = read(fd, buf[fd], BUFF_SIZE);
		if (bytes < 0)
			return (-1); //err = bytes; (man 2 read) -> -1
		if (!bytes)
		{
			if (ft_strlen(*line))
			{
				*(buf[fd]) = '\0';
				return (1);
			}
			free(buf[fd]);
			buf[fd] = NULL;
			return (0); // bytes <= 0; return (bytes);
		}
		buf[fd][bytes] = '\0';
		end_of_line = ft_strchr(buf[fd], '\n');
	}
	*end_of_line = '\0';
	temp = *line;
	*line = ft_strjoin(*line, buf[fd]);
	free(temp);
	temp = buf[fd];
	buf[fd] = ft_strdup(end_of_line + 1);
	free(temp);
	if (!buf[fd])
		return (-1);
	return (1);
}
