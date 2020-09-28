/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_labels.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 16:20:57 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/24 11:40:48 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int					arg_matches_labels(t_token *cur_token, char **arg)
{
	int i;

	i = 0;
	if (!cur_token->label)
		return (0);
	while (cur_token->label[i])
	{
		if (ft_strequ(cur_token->label[i], ft_strchr(*arg, ':') + 1))
			return (1);
		i++;
	}
	return (0);
}

static void			convert_arg_label(t_asm *assm, char **arg, int arg_position)
{
	t_token			*cur_token;
	int				label_position;
	char			*converted_arg;

	label_position = 0;
	cur_token = assm->token;
	while (cur_token)
	{
		if (arg_matches_labels(cur_token, arg))
		{
			if ((*arg)[0] == '%')
				converted_arg = ft_strjoinfree(ft_strdup("%"),
				ft_itoa(label_position - arg_position));
			else
				converted_arg = ft_itoa(label_position - arg_position);
			free(*arg);
			*arg = converted_arg;
			return ;
		}
		label_position += cur_token->size;
		cur_token = cur_token->next;
	}
}

void				convert_labels(t_asm *assm)
{
	t_token			*cur_token;
	int				position;

	position = 0;
	cur_token = assm->token;
	while (cur_token)
	{
		if (cur_token->arg1 && ft_strchr(cur_token->arg1, ':'))
			convert_arg_label(assm, &cur_token->arg1, position);
		if (cur_token->arg2 && ft_strchr(cur_token->arg2, ':'))
			convert_arg_label(assm, &cur_token->arg2, position);
		if (cur_token->arg3 && ft_strchr(cur_token->arg3, ':'))
			convert_arg_label(assm, &cur_token->arg3, position);
		position += cur_token->size;
		cur_token = cur_token->next;
	}
}
