/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 12:44:33 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/04 15:21:27 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void		validation_error(char *message, int row, int col)
{
	ft_printf("Error at [%03d:%03d]: %s.\n", row, col, message);
	exit(1);
}

void		validate_header(t_file *cur, t_validator *vd)
{

	while (cur)
	{
		ft_printf("LINE:: %s\n", cur->line);
		if (cur->line[0] == '.' && ft_strncmp(cur->line, ".name ", 6) && ft_strncmp(cur->line, ".comment ", 9))
			validation_error("Header str not .name or .comment", vd->row, 1);
		if (cur->line[0] == '\n'/*  && name && comment */)
			return ;
		else
		{
			cur = cur->next;
			vd->row++;
		}
	}
	// if starts with . and not ".name" ".comment"

	// if line starts with anything but .#;\n = instruction time

	// exit validate_header after first found nl after both .name & .comment are found.
}

void		validator(t_file *file)
{
	int				row;
	int				col;
	t_file			*cur;
	t_validator		*vd;

	if (!(vd = (t_validator*)ft_memalloc(sizeof(t_validator))))
		handle_error("Malloc failed");
	vd->col = 0;
	vd->row = 0;
	col = 0;
	row = 0;
	cur = file;


	// validate header
	while (cur)
	{
		validate_header(cur, vd);
		col = 0;
		row++;
		cur = cur->next;
	}
	// newline(s) after both name and command have been found.

	// validate instructions
	// while (cur)
	// {
	// 	validate_line(cur->line, col);
	// 	col = 0;
	// 	row++;
	// 	cur = cur->next;
	// }

}
