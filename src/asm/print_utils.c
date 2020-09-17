/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 12:51:38 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 13:14:43 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Various printing functions
*/

void	print_file(t_file *head)
{
	t_file *cur;

	cur = head;
	while (cur)
	{
		ft_printf("%s\n", cur->line);
		cur = cur->next;
	}
}

void	print_tokens(t_token *token)
{
	t_token *cur;

	cur = token;
	while (cur)
	{
		ft_printf("CONVERTED TOKEN\n");
		print_token_info(cur);
		cur = cur->next;
	}
}

void	print_token_info(t_token *token)
{
	ft_printf("LABEL: %s\n", token->label);
	ft_printf("INSTRUCTION: %s\n", token->instruction);
	ft_printf("INSTRUCTION_INDEX: %d\n", token->instruction_index);
	ft_printf("ARG1: %s\n", token->arg1);
	ft_printf("ARG2: %s\n", token->arg2);
	ft_printf("ARG3: %s\n", token->arg3);
	ft_printf("ARGUMENT TYPE CODE: %x\n", token->argument_type_code);
	ft_printf("SIZE: %d\n", token->size);
	ft_printf("\n\n");
}
