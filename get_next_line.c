/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/14 17:05:43 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft/includes/libft.h"
#include <stdlib.h>
#include <unistd.h>

int	get_next_line(const int fd, char **line)
{
	static char	*head;
	char		*end_of_line;
	char		*temp;
	ssize_t		bytes;

	end_of_line = NULL;
	if (head)
		end_of_line = ft_strchr(head, '\n');
	else
		head = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!head)
		return (-1);
	while (!end_of_line)
	{
		*line = ft_strjoin(*line, head);
		bytes = read(fd, head, BUFF_SIZE);
		if (bytes <= 0)
			return (bytes);
		head[bytes] = '\0';
		temp = *line;
		end_of_line = ft_strchr(head, '\n');
		*line = ft_strjoin(*line, head);
		ft_strdel(temp);
	}
}
