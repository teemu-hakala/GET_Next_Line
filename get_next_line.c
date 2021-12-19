/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/19 14:23:49 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	ft_initialize_buffer(t_string **buf)
{
	(*buf)->string = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	if ((*buf)->string)
	{
		*(*buf)->string = '\0';
		(*buf)->string[BUFF_SIZE] = '\0';
	}
	(*buf)->index = 0;
	return (-!(*buf)->string);
}

static int	ft_strjoinfree(char **line, char **string)
{
	char	*del;

	del = *line;
	*line = ft_strjoin(*line, *string);
	free(del);
	return (-!*line);
}

static int	ft_handle_tail(char **line, t_string **buf, char *end_of_line)
{
	char	*del;

	*end_of_line = '\0';
	ft_strjoinfree(line, (*buf)->string);
	(*buf)->index = end_of_line - (*buf)->string;
	if (!*(end_of_line + 1))
	{
		(*buf)->index = 0;
		*(*buf)->string = '\0';
	}
	return (-!*line | 0x1);
}

static int	ft_fill(char **line, t_string **buf, int fd, char *end_of_line)
{
	ssize_t	bytes;

	while (!end_of_line)
	{
		if (ft_strjoinfree(line, &(*buf)->string))
			return (-1);
		(*buf)->string -= (*buf)->index;
		bytes = read(fd, *buf, BUFF_SIZE); //store into ->read (binary)
		if (bytes < 0)
			return (-1);
		if (!bytes)
			return (!!(*buf)->index);
		else if (bytes < BUFF_SIZE)
			(*buf)->string[bytes] = '\0';
		end_of_line = ft_strchr((*buf)->string, '\n');
	}
	return (ft_handle_tail(line, buf, end_of_line));
}

int	get_next_line(const int fd, char **line)
{
	static t_string	*buf[FD_MAX + 1];
	int				result;

	if (fd < 0 || !line || fd > FD_MAX)
		return (-1);
	*line = ft_strnew(0);
	if (!buf[fd] && ft_initialize_buffer(&buf[fd]) == -1)
		return (-1);
	result = ft_fill(line, &buf[fd], fd, ft_strchr(buf[fd]->string, '\n'));
	if (result == -1)
		ft_strdel(line);
	return (result);
}
