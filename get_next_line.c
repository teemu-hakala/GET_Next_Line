/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/14 18:36:53 by thakala          ###   ########.fr       */
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

	*line = ft_strnew(0);
	end_of_line = NULL;
	bytes = 0;
	first = 0;
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
	while (!end_of_line)
	{
		temp = *line;
		*line = ft_strjoin(*line, buf);
		free(temp);
		if (!first && !end_of_line)
			buf = ft_strchr(buf, '\0') - BUFF_SIZE;
		bytes = read(fd, buf, BUFF_SIZE);
		if (bytes <= 0)
			return ((int)bytes);
		buf[bytes] = '\0';
		end_of_line = ft_strchr(buf, '\n');
	}
	*end_of_line = '\0';
	temp = *line;
	*line = ft_strjoin(*line, buf);
	free(temp);
	buf = end_of_line + 1;
	return (1);
}
