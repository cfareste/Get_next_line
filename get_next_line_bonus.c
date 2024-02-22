/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfidalgo <cfidalgo@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:39:11 by cfidalgo          #+#    #+#             */
/*   Updated: 2024/02/22 19:34:02 by cfidalgo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*join_buffers(char **buffer, char *read_buffer, int bytes)
{
	char	*new_buffer;

	read_buffer[bytes] = '\0';
	new_buffer = ft_strjoin(*buffer, read_buffer);
	if (!new_buffer)
		return (NULL);
	free(*buffer);
	*buffer = new_buffer;
	return (new_buffer);
}

char	*slice_buffer(char **buffer, int line_len)
{
	char	*new_buffer;
	int		buffer_len;

	buffer_len = ft_strlen(*buffer);
	new_buffer = ft_calloc(buffer_len - line_len + 1, sizeof(char));
	if (!new_buffer)
		return (NULL);
	ft_strlcpy(new_buffer, *buffer + line_len, buffer_len - line_len + 1);
	free(*buffer);
	*buffer = new_buffer;
	return (new_buffer);
}

char	*extract_line_from_buffer(char **buffer, char *newline_ptr)
{
	char	*line;
	int		line_len;

	if (!newline_ptr)
		line_len = ft_strlen(*buffer);
	else
		line_len = newline_ptr - *buffer + 1;
	line = ft_calloc(line_len + 1, sizeof(char));
	if (!line)
		return (NULL);
	ft_strlcpy(line, *buffer, line_len + 1);
	if ((*buffer)[line_len] == '\0')
	{
		free(*buffer);
		*buffer = NULL;
	}
	else if (!slice_buffer(buffer, line_len))
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*read_from_fd(char **buffer, int fd)
{
	char	*read_buffer;
	char	*newline_ptr;
	int		bytes;

	newline_ptr = ft_strchr(*buffer, '\n');
	if (newline_ptr)
		return (extract_line_from_buffer(buffer, newline_ptr));
	read_buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!read_buffer)
		return (NULL);
	bytes = 1;
	while (bytes > 0 && !newline_ptr)
	{
		bytes = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes < 0 || (bytes && !join_buffers(buffer, read_buffer, bytes)))
			return (free(read_buffer), NULL);
		newline_ptr = ft_strchr(*buffer, '\n');
	}
	free(read_buffer);
	if (!bytes && **buffer == '\0')
		return (NULL);
	return (extract_line_from_buffer(buffer, newline_ptr));
}

char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];
	char		*line;
	int			i;

	i = 0;
	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer[fd])
		return (NULL);
	line = read_from_fd(&buffer[fd], fd);
	if (!line)
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
	}
	return (line);
}
