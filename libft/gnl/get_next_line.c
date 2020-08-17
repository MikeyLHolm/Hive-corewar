/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 16:09:20 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/30 08:55:56 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/get_next_line.h"

static int		lines(char **sta, char **line)
{
	int		len;
	char	*tmp;

	len = 0;
	while ((*sta)[len] != '\n' && (*sta)[len] != '\0')
		len++;
	if ((*sta)[len] == '\n')
	{
		*line = ft_strsub(*sta, 0, len);
		tmp = ft_strdup(&((*sta)[len + 1]));
		free(*sta);
		*sta = tmp;
		if ((*sta)[0] == '\0')
			ft_strdel(sta);
	}
	else
	{
		*line = ft_strdup(*sta);
		ft_strdel(sta);
	}
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	int				ret;
	static char		*sta[FD_SIZE];
	char			buf[BUFF_SIZE + 1];
	char			*tmp;

	if (fd < 0 || fd > FD_SIZE || !line || BUFF_SIZE < 1)
		return (-1);
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if (sta[fd] == NULL)
			sta[fd] = ft_strnew(0);
		tmp = ft_strjoin(sta[fd], buf);
		free(sta[fd]);
		sta[fd] = tmp;
		if (ft_strchr(sta[fd], '\n'))
			break ;
	}
	if (ret < 0)
		return (-1);
	else if (ret == 0 && sta[fd] == NULL)
		return (0);
	else
		return (lines(&sta[fd], line));
}
