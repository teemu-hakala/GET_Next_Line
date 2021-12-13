/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/13 17:10:11 by thakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft/includes/libft.h"
#include <stdlib.h>
#include <unistd.h>

static int	free_buffer_lst(t_list *head)
{
	t_list		*next;
	//Careful! Do not free the whole lst with one failing.
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

static t_buffer	*ft_lstfetch(t_list **head, int fd)
{
	t_buffer	*buf;
	t_list		*previous;

	previous = *head;
	while (*head)
	{
		if (ft_sign(fd) == 1 && (size_t)((t_buffer *)(*head)->content)->fd \
			== ft_abs(fd))
			return ((t_buffer *)(*head)->content);
		else if ((size_t)((t_buffer *)(*head)->content)->fd == ft_abs(fd))
		{
			free(((t_buffer *)(*head)->content)->buf);
			free((t_buffer *)(*head)->content); //ft_lstpop
			return (NULL);
		}
		previous = *head;
		*head = (*head)->next;
	}
	buf = (t_buffer *)malloc(sizeof(t_buffer));
	if (!buf)
		return (NULL);
	buf->fd = fd;
	buf->buf = ft_strnew(0);
	buf->pos = buf->buf;
	buf->count = 0;
	if (*head)
		(*head)->next = ft_lstnew(buf, sizeof(t_buffer));
	else
		*head = ft_lstnew(buf, sizeof(t_buffer));
	return (buf);
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
	size_t			idx;

	ft_lstfetch(&fd_lst, fd);
	buf = (t_buffer *)fd_lst->content;
	if (!buf)
		return (free_buffer_lst(fd_lst));
	//buf contains information still, do not read
	end_of_line = ft_memchr(buf->pos, '\n', BUFF_SIZE * buf->count \
		- (size_t)(buf->pos - buf->buf));
	while (!end_of_line)
	{
		read_bytes = read(buf->fd, stack_buf, BUFF_SIZE);
		if (read_bytes < 0)
			return (-1);
		else if (read_bytes == 0)
			return ((int)ft_lstfetch(&fd_lst, -fd));
		temp = buf->buf;
		idx = (size_t)(buf->pos - buf->buf); //pass as a parameter?
		buf->buf = (char *)ft_memjoin(buf->pos, stack_buf, \
			(buf->count) * BUFF_SIZE - (size_t)(idx), (size_t)read_bytes);
		buf->pos = buf->buf + idx;
		free(temp);
		if (!buf->buf)
			return (free_buffer_lst(fd_lst));
		end_of_line = (char *)ft_memchr(buf->pos, '\n', BUFF_SIZE * \
			(buf->count)++ + (size_t)read_bytes);
	}
	*line = (char *)ft_memdup(buf->pos, (size_t)(end_of_line - buf->pos));
	(*line)[end_of_line - buf->buf] = '\0';
	buf->pos = end_of_line + 1; //protect overrunning
	return (1);
}
