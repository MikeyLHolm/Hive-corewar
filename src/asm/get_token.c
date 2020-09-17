/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 13:45:03 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 13:54:56 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int			get_token_size(t_token *token)
{
	int 		type;
	int 		size;

	size = 1;
	if (g_op_tab[token->instruction_index].args_type_code)
		size += 1;
	type = get_arg_type(token->arg1);
	if (type == REG_CODE)
		size += 1;
	else if (type == IND_CODE)
		size += 2;
	else if (type == DIR_CODE)
		size += (g_op_tab[token->instruction_index].size_t_dir ? 2 : 4);
	type = get_arg_type(token->arg2);
	if (type == REG_CODE)
		size += 1;
	else if (type == IND_CODE)
		size += 2;
	else if (type == DIR_CODE)
		size += (g_op_tab[token->instruction_index].size_t_dir ? 2 : 4);
	type = get_arg_type(token->arg3);
	if (type == REG_CODE)
		size += 1;
	else if (type == IND_CODE)
		size += 2;
	else if (type == DIR_CODE)
		size += (g_op_tab[token->instruction_index].size_t_dir ? 2 : 4);
	return (size);
}

void		get_token_arguments(t_asm *assm, t_token *token)
{
	int			i;
	char		*line;
	char		**args;

	i = 0;
	line = assm->cur->line;
	i = get_first_arg_index(line, token->instruction);
	args = ft_strsplit(&line[i], SEPARATOR_CHAR);
	if (args[0] && args[1] && args[2])
	{
		token->arg3 = ft_strtrim(args[2]);
		free(args[2]);
	}
	if (args[0] && args[1])
	{
		token->arg2 = ft_strtrim(args[1]);
		free(args[1]);
	}
	if (args[0])
	{
		token->arg1 = ft_strtrim(args[0]);
		free(args[0]);
	}
	free(args);
}

char		*get_token_instruction(t_asm *assm)
{
	int 		i;
	int 		j;
	int 		len;

	i = 0;
	while (ft_strchr(LABEL_CHARS, assm->cur->line[i]))
		i++;
	if (assm->cur->line[i] == LABEL_CHAR)
		i++;
	len = 0;
	while (!len)
	{
		while (assm->cur->line[i] && ft_isspace(assm->cur->line[i]))
			i++;
		j = i;
		while (assm->cur->line[j])
		{
			if (ft_isalpha(assm->cur->line[j++]))
				len++;
			else
				break ;
		}
		if (len)
			return (ft_strsub(assm->cur->line, i, len));
		assm->cur = assm->cur->next;
		i = 0;
		while (!ft_isalpha(assm->cur->line[i]))
			i++;
	}
	return (NULL);
}

char		*get_token_label(char *line)
{
	int 		i;

	i = 0;
	while (line[i] && line[i] != LABEL_CHAR)
	{
		if (!ft_strchr(LABEL_CHARS, line[i++]))
			return (NULL);
	}
	// i = 0;
	// while (line[i] != LABEL_CHAR)
	// 	i++;
	return (ft_strsub(line, 0, i));
}
