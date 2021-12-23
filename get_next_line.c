/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/23 19:15:33 by thakala          ###   ########.fr       */
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
	static t_rem	*remnant;
	char			*buf;
	ssize_t			addition;
	char			*newline;
	t_rem			*prev_rem;

	buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!remnant)
		remnant = ft_create_remnant(&buf, 0, 0, NULL);
	prev_rem = remnant;
	if (!size)
	{
		newline = ft_memchr(remnant->str, '\n', remnant->len);
		if (newline)
		{
			*line = (char *)malloc(sizeof(char) * (size_t)(newline - remnant->str + 1));
			ft_memcpy(*line, remnant->str, (size_t)(newline - remnant->str));
			ft_memrplc(*line, (size_t)(newline - remnant->str), '\0', '\n');
			(*line)[newline++ - remnant->str] = '\0';
			ft_create_remnant(&newline, (ssize_t)(remnant->len - (size_t)(newline - remnant->str)), remnant->len, &remnant);
			return ((char *)1);
		}
	}
	newline = ft_read_buffer(fd, &buf, &addition, &remnant);
	if (!newline && addition > 0)
		*line = ft_link_bufs(fd, line, size + BUFF_SIZE);
	else if (newline && addition >= 0)
	{
		//remnant = ft_create_remnant(&newline, addition, remnant->len, &remnant);
		*line = (char *)malloc(sizeof(char) * (size + (size_t)addition + prev_rem->len + 1));
		if (!remnant || !*line)
			return ((char *)(-1));
		(*line)[size + prev_rem->len + (size_t)addition] = '\0';
	}
	else if (!newline && !addition)
		return ((char *)0);
	else
		return ((char *)0);
	ft_memcpy(*line + size + prev_rem->len, buf, (size_t)addition);
	free(buf);
	if (!size)
	{
		ft_memcpy(*line, prev_rem->str, prev_rem->len);
		free(prev_rem);
	}
	return (*line);
}

int	get_next_line(const int fd, char **line)
{
	char	*temp;
	int		result;

	*line = ft_strnew(0);
	temp = *line;
	result = (int)ft_link_bufs(fd, line, 0);
	if (result)
		free(temp);
	if (result == -1)
		return (-1);
	return (!!result);
}
