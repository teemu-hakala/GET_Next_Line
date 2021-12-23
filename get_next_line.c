/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/23 22:34:35 by thakala          ###   ########.fr       */
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
		ft_memdel(&rem);
	}
	if (!rem)
		return (NULL);
	if (!(flag ^ FETCH))
		return (rem);
	rem->len = len;
	previous = rem->str;
	rem->str = (char *)malloc(sizeof(char) * (rem->len + 1));
	ft_memcpy(rem->str, *buf, rem->len);
	free(previous);
	return (rem);
}

static char	*ft_read_buffer(int fd, char **buf, ssize_t *addition)
{

}

static char	*ft_link_bufs(const int fd, char **line, size_t size)
{
	char	*buf;
	char	*newline;
	ssize_t	addition;

	buf = (char *)malloc(sizeof(char) * (BUFF_SIZE/* + 1 */));
	if (!buf)
		return ((char *)(-1));
	ft_read_buffer(fd, &buf, &addition);
	if (addition == -1)
		return ((char *)(-1));
	newline = ft_memchr(buf, '\n', addition);
	if (!newline++)
		*line = ft_link_bufs(fd, line, size + BUFF_SIZE);
	if (addition < BUFF_SIZE)
	{
		ft_remnant_handler(&newline, addition - (newline - 1 - buf), UPDATE);
		*line = (char *)malloc(sizeof(char) * (size + (newline - buf) + 1));
		ft_memrplc(buf, newline - buf, '\0', '\n');
		ft_memcpy(*line + size, buf, newline - buf);
		return (*line);
	}
	ft_memcpy(*line + size, buf, BUFF_SIZE);
}

int	get_next_line(const int fd, char **line)
{
	t_rem	*rem;
	char	*nl;
	long	result;

	if (fd < 0 || !line)
		return (-1);
	rem = ft_remnant_handler(NULL, 0, FETCH);
	nl = ft_memchr(rem->str, '\n', rem->len);
	if (nl)
	{
		ft_memcpy(*line, rem->str, nl++ - rem->str);
		ft_remnant_handler(&nl, rem->len - (nl - rem->str) - 1, UPDATE); //
		return (1);
	}
	result = (long)ft_link_bufs(fd, line, rem->len); // , 0)
	ft_memcpy(*line, rem->str, rem->len);
	ft_memrplc(*line, rem->len, '\0', '\n');
	ft_remnant_handler("", 0, UPDATE);
	if (result == -1)
		return (result);
	return (!!result);
}
