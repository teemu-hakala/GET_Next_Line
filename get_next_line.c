/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/10 11:01:11 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft/includes/libft.h"

static int	free_buffer_lst(t_list *head)
{
	t_list		*next;

	while (head)
	{
		free(((t_buffer *)head->content)->buf);
		next = head->next;
		free(head);
		head = next;
	}
	return (-1);
}

/*
	Negative file descriptor fd will remove node, positive one will add.
*/

static char	*ft_lstfetch(t_list *head, int fd)
{
	t_buffer	*buf;

	while (head)
	{
		if (((t_buffer *)head->content)->fd == fd)
			return (((t_buffer *)head->content)->buf);
		head = head->next;
	}
	buf = (t_buffer *)malloc(sizeof(t_buffer));
	if (!buf)
		return (NULL);
	buf->fd = fd;
	buf->buf = ft_strnew(0);
	buf->pos = buf->buf;
	head->next = ft_lstnew(buf, sizeof(t_buffer));
}

/*
	temp_line = (char *)ft_memdup(buf->buf, end_of_line - buf->buf + 2);
*/

int	get_next_line(const int fd, char **line)
{
	static t_list	*fd_lst;
	t_buffer		*buf;
	char			stack_buf[BUFF_SIZE];
	ssize_t			read_bytes;
	char			*end_of_line;
	char			*temp;
	char			*heap_buf;
	char			*temp_line;

	buf = ft_lstfetch(fd_lst, fd);
	if (!buf)
		return (free_buffer_lst(fd_lst));
	while (1)
	{
		read_bytes = read(buf->fd, stack_buf, BUFF_SIZE);
		if (read_bytes < 0)
			return (-1);
		temp = buf->buf;
		buf->buf = (char *)ft_memjoin(buf->pos, stack_buf, \
			buf->count * BUFF_SIZE, read_bytes);
		free(temp);
		if (!buf->buf)
			return (free_buffer_lst(fd_lst));
		end_of_line = (char *)ft_memchr(buf, '\n', read_bytes);
		if (end_of_line)
		{
			temp_line = (char *)ft_memdup(buf->buf, end_of_line - buf->buf + 2);
			temp_line[end_of_line - buf->buf] = '\0';
			buf->pos = end_of_line + 1;
			*line = temp_line;
		}
	}
}
