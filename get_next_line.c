/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/14 17:32:33 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft/includes/libft.h"
#include <stdlib.h>
#include <unistd.h>

int	get_next_line(const int fd, char **line)
{
	static t_buf	*head;
	char			*end_of_line;
	ssize_t			bytes;

	*line = NULL;
	end_of_line = NULL;
	if (!head)
	{
		head = (t_buf *)malloc(sizeof(t_buf));
		if (!head)
			return (-1);
		head->buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
		if (!head->buf)
			return (-1);
		head->pos = head->buf;
	}
	else
		end_of_line = ft_strchr(head->pos, '\n');
	if (end_of_line)
		*end_of_line = '\0';
	while (!end_of_line)
	{
		bytes = read(fd, head->buf, BUFF_SIZE);
		head->buf[bytes] = '\0';
		end_of_line = ft_strchr(*line, '\n');
		if (end_of_line)
		{
			end_of_line = '\0';
			head->pos = end_of_line + 1;
		}
	}
}
