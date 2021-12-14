/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/14 18:26:57 by thakala          ###   ########.fr       */
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

	*line = ft_strnew(0);
	end_of_line = NULL;
	if (!buf)
	{
		buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
		if (!buf)
			return (-1);
	}
	else
		end_of_line = ft_strchr(buf, '\n');
	while (!end_of_line)
	{
		temp = *line;
		*line = ft_strjoin(*line, buf);
		free(temp);
		bytes = read(fd, buf, BUFF_SIZE);
		if (bytes <= 0)
			return ((int)bytes);
		buf[bytes] = '\0';
		end_of_line = ft_strchr(buf, '\n');
		/*if (!end_of_line)
			buf = ft_strchr(buf, '\0') - bytes;*/
	}
	*end_of_line = '\0';
	temp = *line;
	*line = ft_strjoin(*line, buf);
	free(temp);
	buf = end_of_line + 1;
	return (1);
}
