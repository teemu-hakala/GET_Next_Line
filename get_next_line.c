/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/24 16:47:15 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
	Adding nul byte '\0' is redundant since memchr is used?
	return (((char *)ft_memcpy(rem->str, *buf, rem->len))[rem->len] = '\0');
*/

static t_rem	*ft_remnant_handler(char **buf, size_t len, size_t flag)
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
		return (rem);
	rem->len = len;
	previous = rem->str;
	rem->str = (char *)malloc(sizeof(char) * (rem->len/* + 1*/));
	ft_memcpy(rem->str, *buf, rem->len);
	free(previous);
	return (rem);
}

static void	ft_read_buffer(int fd, char **buf, ssize_t *addition)
{
	*addition = read(fd, *buf, BUFF_SIZE);
}

static char	*ft_link_bufs(const int fd, char **line, size_t size)
{
	char	*buf;
	char	*nl;
	ssize_t	addition;

	buf = (char *)malloc(sizeof(char) * (BUFF_SIZE/* + 1 */));
	if (!buf)
		return ((char *)(-1));
	/*size += */ft_read_buffer(fd, &buf, &addition);
	if (addition <= 0)
		return ((char *)(addition));
	nl = ft_memchr(buf, '\n', (size_t)addition);
	if (!nl)
		*line = ft_link_bufs(fd, line, size + BUFF_SIZE);
	if (!nl++ && addition == BUFF_SIZE)
		return (ft_memcpy(*line + size, buf, BUFF_SIZE) - size);
	ft_remnant_handler(&nl, (size_t)(addition - (nl - buf)), UPDATE);
	*line = (char *)malloc(sizeof(char) * (size + (size_t)(--nl - buf) + 1));
	ft_memrplc(buf, (size_t)(nl - buf), '\0', '\n');
	ft_memcpy(*line + size, buf, (size_t)(nl - buf));
	(*line)[size + (size_t)(nl - buf)] = '\0';
	return (*line);
}

int	get_next_line(const int fd, char **line)
{
	t_rem	*rem;
	t_rem	*tmp;
	char	*nl;
	long	result;

	if (fd < 0 || !line)
		return (-1);
	rem = ft_remnant_handler(NULL, 0, FETCH);
	nl = ft_memchr(rem->str, '\n', rem->len);
	if (nl)
	{
		*line = (char *)malloc(sizeof(char) * ((size_t)(nl - rem->str) + 1));
		if (!*line)
			return (-1);
		(*line)[(size_t)(nl - rem->str)] = '\0';
		ft_memcpy(*line, rem->str, (size_t)(nl++ - rem->str));
		ft_remnant_handler(&nl, rem->len - (size_t)(nl - rem->str), UPDATE); //
		return (1);
	}
	tmp = (t_rem *)malloc(sizeof(t_rem *));
	if (!tmp)
		return (-1);
	tmp->str = ft_memdup(rem->str, rem->len);
	tmp->len = rem->len;
	if (!tmp->str)
		return (-1);
	tmp->str[rem->len] = '\0';
	result = (long)ft_link_bufs(fd, line, rem->len); // , 0)
	if (!tmp->len && !result)
		return ((int)ft_remnant_handler(NULL, 0, CLEAR));
	ft_memcpy(*line, tmp->str, tmp->len); //rem has been free'ed...
	ft_memrplc(*line, tmp->len, '\0', '\n');
	//ft_remnant_handler((char **)&"", 0, UPDATE);
	free(tmp->str);
	free(tmp);
	if (result == -1)
		return (-1);
	return (!!result);
}
