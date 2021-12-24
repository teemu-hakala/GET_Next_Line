/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/25 00:16:26 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_rem	**ft_remnant(char **buf, size_t len, char *idx, size_t flag)
{
	static t_rem	*rem;
	char			*previous;

	if (!rem)
		rem = (t_rem *)ft_memalloc(sizeof(t_rem));
	if (!(flag ^ CLEAR))
	{
		ft_strdel(&rem->str);
		ft_memdel((void **)&rem);
	}
	if (!rem)
		return (NULL);
	if (!(flag ^ FETCH))
		return (&rem);
	rem->len = len;
	rem->idx = idx;
	previous = rem->str;
	rem->str = *buf;
	free(previous);
	return (&rem);
}

static char	*ft_link_bufs(const int fd, char **line, size_t size)
{
	char	*buf;
	char	*nl;
	ssize_t	addition;

	buf = (char *)malloc(sizeof(char) * (BUFF_SIZE));
	if (!buf)
		return ((char *)(-1));
	addition = read(fd, buf, BUFF_SIZE);
	if (addition <= 0)
		return ((char *)(addition));
	nl = ft_memchr(buf, '\n', (size_t)addition);
	if (!nl)
		*line = ft_link_bufs(fd, line, size + BUFF_SIZE);
	if (!nl && addition == BUFF_SIZE)
		return (ft_memcpy(*line + size, buf, BUFF_SIZE) - size);
	*line = (char *)malloc(sizeof(char) * (size + (size_t)(nl - buf) + 1));
	ft_memcpy(*line, (*ft_remnant(NULL, 0, 0, FETCH))->str, size % BUFF_SIZE);
	ft_remnant(buf, (size_t)addition, (size_t)(nl - buf + 1), UPDATE);
	ft_memcpy(*line + size, buf, (size_t)(nl - buf));
	ft_memrplc(*line, (size_t)(nl - buf), '\0', '\n');
	(*line)[size + (size_t)(nl - buf)] = '\0';
	return (*line);
}

int	get_next_line(const int fd, char **line)
{
	t_rem	**r;
	char	*nl;
	long	result;

	if (fd < 0 || !line)
		return (-1);
	r = ft_remnant(NULL, 0, NULL, FETCH);
	nl = ft_memchr((*r)->idx, '\n', (*r)->len - ((*r)->idx - (*r)->str));
	if (nl)
	{
		*line = (char *)malloc(sizeof(char) * ((size_t)(nl - (*r)->idx) + 1));
		if (!*line)
			return (-1);
		(*line)[(size_t)(nl - (*r)->str)] = '\0';
		ft_memcpy(*line, (*r)->str, (size_t)(nl - (*r)->str));
		(*r)->idx = nl + (nl - (*r)->str != (*r)->len);
		return (1);
	}
	result = (long)ft_link_bufs(fd, line, (*r)->len);
	if (!result)
		return ((int)ft_remnant(NULL, 0, 0, CLEAR));
	if (result == -1)
		return (-1);
	return (!!result);
}
