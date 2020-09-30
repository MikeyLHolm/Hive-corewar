/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_labels.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 13:45:03 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 11:17:11 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

char	*get_token_label(char *line)
{
	int				i;

	i = 0;
	while (line[i] && line[i] != LABEL_CHAR)
	{
		if (!ft_strchr(LABEL_CHARS, line[i++]))
			return (NULL);
	}
	return (ft_strsub(line, 0, i));
}

int		get_2d_array_size(char **arr)
{
	int i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

char	**resize_2d_array(char **arr, char *str)
{
	char	**new_arr;
	int		i;

	if (!arr)
	{
		new_arr = (char**)ft_memalloc(sizeof(char*) * 2);
		new_arr[0] = str;
	}
	else
	{
		new_arr = (char**)ft_memalloc(sizeof(char*) * (get_2d_array_size(arr)
			+ 2));
		i = 0;
		while (i < get_2d_array_size(arr))
		{
			new_arr[i] = arr[i];
			i++;
		}
		new_arr[i] = str;
		free(arr);
	}
	return (new_arr);
}

char	**revert_file_line(t_asm *assm, t_file *prev, char **arr)
{
	assm->cur = prev;
	return (arr);
}

char	**get_token_labels(t_asm *assm)
{
	int		i;
	char	**arr;
	t_file	*prev;

	arr = NULL;
	i = 0;
	prev = assm->cur;
	while (1)
	{
		if (!assm->cur)
			return (revert_file_line(assm, prev, arr));
		while (assm->cur->line[i] && assm->cur->line[i] != LABEL_CHAR)
			if (!ft_strchr(LABEL_CHARS, assm->cur->line[i++]))
				return (revert_file_line(assm, prev, arr));
		if (assm->cur->line[i] == LABEL_CHAR)
			arr = resize_2d_array(arr, ft_strsub(assm->cur->line, 0, i));
		while (assm->cur->line[++i])
			if (!ft_isspace(assm->cur->line[i]))
				break ;
		if (assm->cur->line[i])
			return (revert_file_line(assm, prev, arr));
		prev = assm->cur;
		assm->cur = assm->cur->next;
		i = 0;
	}
}
