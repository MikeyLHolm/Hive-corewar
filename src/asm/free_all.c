/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 10:27:42 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 12:50:47 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void		free_label(t_label *head)
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

void		free_file(t_file *head)
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

void		free_validator(t_validator *vd)
{
	free(vd->comment);
	free(vd->name);
	free_label(vd->label);
	free(vd);
}

void		free_assm(t_asm *assm)
{
	free(assm->comment);
	free(assm->name);
	free_file(assm->file);
}
