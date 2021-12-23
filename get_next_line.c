/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/23 21:48:08 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_rem	*ft_remnant_handler(char **buf, size_t bytes, size_t flag)
{
	static t_rem	*remnant;

	if (!remnant)
		remnant = (t_rem *)malloc(sizeof(t_rem));
	if (!(flag ^ CLEAR))
	{
		ft_strdel(&remnant->str);
		ft_memdel(&remnant);
	}
	if (!remnant)
		return (NULL);
	if (!(flag ^ FETCH))
		return (remnant);
	//else //if (flag == UPDATE)
	remnant->str = (char *)malloc(sizeof(char) * (bytes + 1));
	((char *)ft_memcpy(remnant->str, buf, bytes))[bytes] = '\0';
}

static char	*ft_read_buffer(int fd, char **buf, ssize_t *addition)
{

}

static char	*ft_link_bufs(const int fd, char **line, size_t size)
{

}

int	get_next_line(const int fd, char **line)
{
	ft_remnant_handler(NULL, 0, FETCH);
}
