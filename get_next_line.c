/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/23 20:27:51 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_rem	*ft_create_remnant(char **buf, ssize_t remainder, \
	size_t previous_len, t_rem **previous)
{
	t_rem	*remnant;

	(void)previous_len;
	remnant = (t_rem *)malloc(sizeof(t_rem));
	if (!remnant)
		return (NULL);
	remnant->len = (size_t)remainder; //(size_t)(BUFF_SIZE - addition - 1);
	remnant->str = (char *)malloc(sizeof(char) * (remnant->len + 1));
	if (!remnant->str)
		return (NULL);
	ft_memcpy(remnant->str, *buf, remnant->len);
	remnant->str[remnant->len] = '\0';
	if (previous)
	{
		if (*previous)
		{
			//free((*previous)->str);
			////free(previous);
		}
		*previous = remnant;
	}
	return (remnant);
}

static char	*ft_read_buffer(int fd, char **buf, ssize_t *addition, \
	t_rem **remnant)
{
	char	*newline;

	*addition = read(fd, *buf, BUFF_SIZE);
	if (*addition <= 0)
		return (NULL);
	newline = ft_memchr(*buf, '\n', (size_t)*addition);
	if (newline++)
	{
		*remnant = ft_create_remnant(&newline, *addition - (newline - *buf), (*remnant)->len, remnant);
		*addition = (ssize_t)(newline - 1 - *buf);
		((char *)ft_memrplc(*buf, (size_t)*addition, '\0', '\n'))[(size_t)*addition] = '\0';
	}
	return (newline - 1);
}

static char	*ft_link_bufs(const int fd, char **line, size_t size)
{
	static t_rem	*remnant[FD_MAX + 1];
	char			*buf;
	ssize_t			addition;
	char			*newline;
	t_rem			*prev_rem;

	buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!remnant[fd])
		remnant[fd] = ft_create_remnant(&buf, 0, 0, NULL);
	prev_rem = remnant[fd];
	if (!size)
	{
		newline = ft_memchr(remnant[fd]->str, '\n', remnant[fd]->len);
		if (newline)
		{
			*line = (char *)malloc(sizeof(char) * (size_t)(newline - remnant[fd]->str + 1));
			ft_memcpy(*line, remnant[fd]->str, (size_t)(newline - remnant[fd]->str));
			ft_memrplc(*line, (size_t)(newline - remnant[fd]->str), '\0', '\n');
			(*line)[newline++ - remnant[fd]->str] = '\0';
			ft_create_remnant(&newline, (ssize_t)(remnant[fd]->len - (size_t)(newline - remnant[fd]->str)), remnant[fd]->len, &remnant[fd]);
			return ((char *)1);
		}
	}
	newline = ft_read_buffer(fd, &buf, &addition, &remnant[fd]);
	if (!newline && addition == BUFF_SIZE)
		*line = ft_link_bufs(fd, line, size + BUFF_SIZE);
	else if (newline && addition >= 0)
	{
		//remnant = ft_create_remnant(&newline, addition, remnant->len, &remnant);
		*line = (char *)malloc(sizeof(char) * (size + (size_t)addition + prev_rem->len + 1));
		if (!remnant[fd] || !*line)
			return ((char *)(-1));
		(*line)[size + prev_rem->len + (size_t)addition] = '\0';
	}
	else if (!newline && !addition)
	{
		free(buf);
		return ((char *)0);
	}
	else
	{
		*line = (char *)malloc(sizeof(char) * (size_t)(addition + 1)); //return ((char *)0);
		(*line)[addition] = '\0';
	}
	ft_memcpy(*line + size + prev_rem->len, buf, (size_t)addition);
	free(buf);
	if (!size)
	{
		ft_memcpy(*line, prev_rem->str, prev_rem->len);
		//free(prev_rem);
	}
	return (*line);
}

int	get_next_line(const int fd, char **line)
{
	char	*temp;
	long	result;

	if (fd < 0 || !line || fd > FD_MAX)
		return (-1);
	*line = ft_strnew(0);
	temp = *line;
	result = (long)ft_link_bufs(fd, line, 0);
	if (result)
		free(temp);
	if (result == -1)
		return (-1);
	return (!!result);
}
