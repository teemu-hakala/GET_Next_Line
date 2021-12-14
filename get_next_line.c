/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thakala <thakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:21:02 by thakala           #+#    #+#             */
/*   Updated: 2021/12/14 15:03:38 by thakala          ###   ########.fr       */
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
	t_list		*new;

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
	buf->rem = 0;
	new = ft_lstnew(buf, sizeof(t_buffer));
	if (!new)
		return (NULL);
	if (*head)
		(*head)->next = new;
	else
		*head = new;
	return (buf);
}

/*
	temp_line = (char *)ft_memdup(buf->buf, end_of_line - buf->buf + 2);
*/

/*
	Push buffer's to line. Even heap buffer size should stay?
*/

int	get_next_line(const int fd, char **line)
{
	static t_list	*fd_lst;
	t_buffer		*buf;
	char			stack_buf[BUFF_SIZE];
	ssize_t			bytes;
	char			*end_of_line;
	char			*temp;

	ft_lstfetch(&fd_lst, fd); // do not alter fd_lst
	buf = (t_buffer *)fd_lst->content;
	if (!buf)
		return (free_buffer_lst(fd_lst));
	end_of_line = ft_memchr(buf->pos, '\n', buf->rem);
	if (!end_of_line)
		buf->count = 0;
	while (!end_of_line)
	{
		bytes = read(buf->fd, stack_buf, BUFF_SIZE);
		if (bytes < 0)
			return (-1);
		else if (bytes == 0)
			return ((int)ft_lstfetch(&fd_lst, -fd));
		temp = buf->buf; //pass as a parameter?
		buf->buf = (char *)ft_memjoin(buf->pos, stack_buf, \
			buf->rem, (size_t)bytes);
		buf->rem += (size_t)bytes;
		buf->count++;
		buf->pos = buf->buf;
		ft_strdel(&temp);
		if (!buf->buf)
			return (free_buffer_lst(fd_lst));
		end_of_line = (char *)ft_memchr(buf->pos, '\n', buf->rem);
	}
	*line = (char *)ft_memdup(buf->pos, (size_t)(end_of_line - buf->pos));
	(*line)[end_of_line - buf->buf] = '\0';
	buf->rem -= (size_t)(end_of_line + 1 - buf->pos);
	buf->pos = end_of_line + 1; //protect? (-> memchr won't run with rem 0)
	return (1);
}
