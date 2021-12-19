/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/19 12:05:45 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	ft_initialize_buffer(char **buf)
{
	free(*buf);
	*buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (*buf)
		**buf = '\0';
	return (-!*buf);
}

static int	ft_strjoinfree(char **begin, char **append)
{
	char	*del;

	del = *begin;
	*begin = ft_strjoin(*begin, *append);
	free(del);
	return (-!*begin);
}

static int	ft_handle_tail(char **line, char **buf, char *end_of_line)
{
	char	*del;

	*end_of_line = '\0';
	ft_strjoinfree(line, buf);
	del = *buf;
	*buf = ft_strdup(end_of_line + 1);
	free(del);
	return (-!(*buf && *line) | 0x1);
}

static int	ft_fill(char **line, char **buf, int fd, char *end_of_line)
{
	ssize_t	bytes;

	bytes = 0;
	while (!end_of_line)
	{
		if (ft_strjoinfree(line, buf) || (!bytes && ft_initialize_buffer(buf)))
			return (-1);
		bytes = read(fd, *buf, BUFF_SIZE);
		if (bytes < 0)
			return (-1);
		if (!bytes)
		{
			if (ft_strlen(*line))
			{
				**buf = '\0';
				return (1);
			}
			ft_strdel(buf);
			ft_strdel(line);
			return (0);
		}
		(*buf)[bytes] = '\0';
		end_of_line = ft_strchr(*buf, '\n');
	}
	return (ft_handle_tail(line, buf, end_of_line));
}

int	get_next_line(const int fd, char **line)
{
	static char	*buf[FD_MAX + 1];
	int			result;

	if (fd < 0 || !line || fd > FD_MAX)
		return (-1);
	*line = ft_strnew(0);
	if (!buf[fd])
		if (ft_initialize_buffer(&buf[fd]))
			return (-1);
	result = ft_fill(line, &buf[fd], fd, ft_strchr(buf[fd], '\n'));
	if (result == -1)
		ft_strdel(line);
	return (result);
}
