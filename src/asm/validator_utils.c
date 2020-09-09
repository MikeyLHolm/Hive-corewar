/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 16:54:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/09 16:47:59 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void		validation_error(char *message, int row, int col)
{
	ft_printf("Error at [%03d:%03d]: %s.\n", row, col, message);
	exit(1);
}

t_file		*increment_validator(t_file *cur, t_validator *vd)
{
	cur = cur->next;
	++vd->row;
	return (cur);
}

/*
**	Auxilliary function to display LL recursively.
*/

void		display_list(t_label *head)
{
	if (head != NULL)
	{
		ft_printf("\nLabel:: [%s]\n", head->label_name);
		display_list(head->next);
	}
}
