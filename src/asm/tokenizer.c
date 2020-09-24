/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 13:40:07 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/24 15:41:15 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_token		*new_token(t_asm *assm)
{
	t_token		*token;

	if (!(token = (t_token*)ft_memalloc(sizeof(t_token))))
		handle_error("Malloc failed");
	token->label = get_token_label(assm->cur->line);
	token->instruction = get_token_instruction(assm, 0);
	token->instruction_index = get_instruction_index(token->instruction);
	get_token_arguments(assm, token);
	token->argument_type_code = get_argument_type_code(token);
	token->size = get_token_size(token);
	assm->champion_size += token->size;
	//print_token_info(token);
	return (token);
}

void		tokenize_file(t_asm *assm)
{
	t_token		*cur_token;

	get_name_and_comment(assm);
	assm->cur = assm->file;
	while (assm->cur->line && assm->cur->line[0] == '.')
		assm->cur = assm->cur->next;
	assm->cur = assm->cur->next;
	while (assm->cur)
	{
		if (line_contains_instruction(assm->cur))
		{
			if (!assm->token)
			{
				cur_token = new_token(assm);
				if (!cur_token)
					return ;
				assm->token = cur_token;
			}
			else
			{
				cur_token->next = new_token(assm);
				cur_token = cur_token->next;
			}
		}
		if (assm->cur)
			assm->cur = assm->cur->next;
	}
}
