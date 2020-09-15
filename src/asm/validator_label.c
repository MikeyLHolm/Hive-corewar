/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_label.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 14:48:17 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/15 09:38:55 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Creates new label during save_labels.
*/

static t_label		*new_label(char *labelname)
{
	t_label		*new;

	if (!(new = (t_label*)ft_memalloc(sizeof(t_label))))
		handle_error("Malloc failed while creating new label");
	new->label_name = labelname;
	return (new);
}

/*
**	Checking that label is from list of real labels
*/

void				validate_label(char *label, int row, t_label *head)
{
	int			i;
	t_label		*cur;

	//ft_printf("LABEL [%s]\n", label);
	cur = head;
	while (cur)
	{
		if (ft_strequ(label, cur->label_name))
			return ;
		cur = cur->next;
	}
	i = -1;
	while (++i < OP_CODE_AMOUNT)
	{
		if (ft_strequ(label, g_op_tab[i].op_name))
			return ;
	}
	validation_error("Not a real label/statement", row);
}

/*
**	Save labels before validating instructions.
**	Since labels can be called before they appear in instructions.
*/

t_label				*save_labels(t_file *head, t_validator *vd)
{
	t_file		*cur;
	t_label		*label;

	cur = head;
	while (cur)
	{
		if (ft_strchr(LABEL_CHARS, cur->line[0]) && cur->line[0] &&
		cur->line[0] != ALT_COMMENT_CHAR && cur->line[0] != COMMENT_CHAR)
		{
			if (!vd->label)
			{
				label = new_label(get_token_label(cur->line));
				vd->label = label;
			}
			else
			{
				label->next = new_label(get_token_label(cur->line));
				label = label->next;
			}
			if (!label->label_name)
				validation_error("Not a valid label", vd->row);
		}
		cur = increment_validator(cur, vd);
	}
	return (vd->label);
}
