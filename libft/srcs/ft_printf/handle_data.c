/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 14:26:55 by sadawi            #+#    #+#             */
/*   Updated: 2020/03/28 20:59:40 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

t_data	*join_data(t_data *data1, t_data *data2)
{
	t_data *new_data;

	new_data = (t_data*)ft_memalloc(sizeof(t_data));
	new_data->amount = data1->amount + data2->amount;
	new_data->output = ft_strjoin(data1->output, data2->output);
	free(data1);
	free(data2);
	return (new_data);
}

void	init_data(t_data **data)
{
	*data = (t_data*)ft_memalloc(sizeof(t_data));
	(*data)->amount = 0;
}

char	*join_char_to_str(char *str, char c)
{
	char *newstr;

	newstr = (char*)ft_strnew(ft_strlen(str) + 2);
	newstr = ft_strcpy(newstr, str);
	newstr[ft_strlen(newstr)] = c;
	free(str);
	return (newstr);
}
