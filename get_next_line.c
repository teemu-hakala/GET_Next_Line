/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/25 00:50:24 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_rem	**ft_remnant(char *buf, size_t len, char *idx, size_t flag)
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
	rem->i = idx;
	previous = rem->str;
	rem->str = buf;
	free(previous);
	return (&rem);
}

static char	*ft_link(const int fd, char **line, size_t size)
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
		*line = ft_link(fd, line, size + BUFF_SIZE);
	if (!nl && addition == BUFF_SIZE)
		return (ft_memrplc(ft_memcpy(*line + size, buf, BUFF_SIZE), BUFF_SIZE, \
			'\0', '\n') - size);
	*line = (char *)malloc(sizeof(char) * (size + (size_t)(nl - buf) + 1));
	ft_memcpy(*line, (*ft_remnant(NULL, 0, 0, FETCH))->i, size % BUFF_SIZE);
	ft_remnant(buf, (size_t)addition, nl + 1, UPDATE);
	ft_memcpy(*line + size, buf, (size_t)(nl - buf));
	ft_memrplc(*line + size, (size_t)(nl - buf), '\0', '\n');
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
	nl = ft_memchr((*r)->i, '\n', (*r)->len - (size_t)((*r)->i - (*r)->str));
	if (nl)
	{
		*line = (char *)malloc(sizeof(char) * ((size_t)(nl - (*r)->i) + 1));
		if (!*line)
			return (-1);
		(*line)[(size_t)(nl - (*r)->i)] = '\0';
		ft_memcpy(*line, (*r)->i, (size_t)(nl - (*r)->i));
		(*r)->i = nl + ((size_t)(nl - (*r)->str) != (*r)->len);
		return (1);
	}
	result = (long)ft_link(fd, line, (*r)->len - (size_t)((*r)->i - (*r)->str));
	if (!result)
		return ((int)ft_remnant(NULL, 0, 0, CLEAR));
	if (result == -1)
		return (-1);
	return (!!result);
}
