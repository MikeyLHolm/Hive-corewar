/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 16:54:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 12:48:48 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Exits with error message and row number for the error
*/

void		validation_error(char *message, int row)
{
	ft_printf("Error at row [%03d]: %s.\n", row, message);
	exit(1);
}

/*
**	Moves to next node and line
*/

t_file		*increment_validator(t_file *cur, t_validator *vd)
{
	cur = cur->next;
	++vd->row;
	return (cur);
}

/*
**	Auxilliary functions to display LL for debugging.
*/

void		display_list(t_label *head)
{
	t_label		*cur;

	cur = head;
	while (cur != NULL)
	{
		ft_printf("Label:: [%s]\n", cur->label_name);
		cur = cur->next;
	}
}

void		display_filelist(t_file *head)
{
	t_file		*cur;

	cur = head;
	while (cur != NULL)
	{
		ft_printf("Current line:: [%s]\n", cur->line);
		cur = cur->next;
	}
}
