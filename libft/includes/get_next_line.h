/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 14:36:39 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/23 13:16:02 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFF_SIZE 128

typedef struct		s_fd
{
	char			*str;
	int				fd;
	struct s_fd		*next;
}					t_fd;

int					get_next_line(const int fd, char **line);

#endif
