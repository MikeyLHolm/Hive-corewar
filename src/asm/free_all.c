/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 10:27:42 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 10:34:11 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static void		free_label(t_label *head)
{
	t_label		*cur;
	t_label		*tmp;

	cur = head;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->label_name);
		free(tmp);
	}
}

static void		free_file(t_file *head)
{
	t_file		*cur;
	t_file		*tmp;

	cur = head;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->line);
		free(tmp);
	}
}

static void		free_tokens(t_asm *assm)
{
	t_token		*cur;
	t_token		*tmp;
	int			i;

	cur = assm->token;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->arg1);
		free(tmp->arg2);
		free(tmp->arg3);
		free(tmp->instruction);
		i = 0;
		while (tmp->label && tmp->label[i])
			free(tmp->label[i++]);
		free(tmp->label);
		free(tmp);
	}
}

void			free_validator(t_validator *vd)
{
	free_label(vd->label);
	free(vd);
}

void			free_assm(t_asm *assm)
{
	free(assm->comment);
	free(assm->name);
	free_file(assm->file);
	free_tokens(assm);
	free(assm);
}
