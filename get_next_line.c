/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/21 21:52:07 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_rem	*ft_create_remnant(char **buf, ssize_t addition)
{
	t_rem	*remnant;

	remnant = (t_rem *)malloc(sizeof(t_rem));
	if (!remnant)
		return (NULL);
	remnant->len = (size_t)(BUFF_SIZE - addition - 1);
	remnant->str = (char *)malloc(sizeof(char) * (remnant->len + 1));
	if (!remnant->str)
		return (NULL);
	ft_memcpy(remnant->str, *buf, remnant->len);
	remnant->str[remnant->len] = '\0';
	return (remnant);
}

static char	*ft_read_buffer(int fd, char **buf, ssize_t *addition)
{
	char	*newline;

	*addition = read(fd, *buf, BUFF_SIZE);
	if (addition <= 0)
		return (NULL);
	(*buf)[*addition] = '\0';
	newline = ft_memchr(*buf, '\n', (size_t)*addition);
	if (newline)
		*addition = /*addition -*/ (ssize_t)(newline - *buf);
	return (newline);
}

static char	*ft_link_bufs(const int fd, char **line, size_t size)
{
	static t_rem	*remnant;
	char			*buf;
	ssize_t			addition;
	char			*newline;
	size_t			old_len;
	

	buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!remnant)
		remnant = ft_create_remnant(&buf, BUFF_SIZE - 1);
	old_len = remnant->len;
	if (!size)
	{
		newline = ft_memchr(remnant->str, '\n', remnant->len);
		if (newline)
		{
			*line = (char *)malloc(sizeof(char) * (size_t)(newline - remnant->str + 1));
			ft_memcpy(*line, remnant->str, (size_t)(newline - remnant->str));
			ft_create_remnant(&newline, (ssize_t)(remnant->len - (size_t)(newline - remnant->str)));
			return ((char *)(-1));
		}
	}
	newline = ft_read_buffer(fd, &buf, &addition);
	if (!newline++ && addition > 0)
		*line = ft_link_bufs(fd, line, size + BUFF_SIZE);
	else if (addition > 0)
	{
		remnant = ft_create_remnant(&newline, addition);
		*line = (char *)malloc(sizeof(char) * (size + (size_t)addition + 1));
		if (!remnant || !*line)
			return (NULL);
		(*line)[size + old_len + (size_t)addition] = '\0';
	}
	else
		return (NULL);
	ft_memcpy(*line + size + old_len, buf, (size_t)addition);
	free(buf);
	if (!size)
		ft_memcpy(*line, remnant->str, old_len);
	return (*line);
}

int	get_next_line(const int fd, char **line)
{
	char	*temp;
	*line = ft_strnew(0);
	temp = *line;
	ft_link_bufs(fd, line, 0);
	free(temp);
	return (1);
}
