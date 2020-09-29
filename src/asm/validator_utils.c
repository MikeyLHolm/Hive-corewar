/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 16:54:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 16:45:40 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Exits with error message and row number for the error
*/

void		validation_error(char *message, int row)
{
	ft_fprintf(2, "Error at row [%03d]: %s.\n", row, message);
	exit(1);
}

/*
**	skips label and following :
*/

int			skip_label(char *line)
{
	int			i;

	i = 0;
	while (ft_strchr(LABEL_CHARS, line[i]))
		i++;
	if (line[i] == LABEL_CHAR)
		i++;
	else
		i = 0;
	return (i);
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

void		print_list(t_label *head)
{
	t_label		*cur;

	cur = head;
	while (cur != NULL)
	{
		ft_printf("Label:: [%s]\n", cur->label_name);
		cur = cur->next;
	}
}

void		print_filelist(t_file *head)
{
	t_file		*cur;

	cur = head;
	while (cur != NULL)
	{
		ft_printf("Current line:: [%s]\n", cur->line);
		cur = cur->next;
	}
}
