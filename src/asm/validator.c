/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 12:44:33 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/04 15:46:30 by mlindhol         ###   ########.fr       */
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
		// ft_printf("LINE:: %s\n", cur->line);
		// Check that line starting with . is either .name or .comment
		if (!ft_strncmp(cur->line, ".name ", 6))
			vd->data = vd->data | HEADER_NAME;
		else if (!ft_strncmp(cur->line, ".comment ", 9))
			vd->data = vd->data | HEADER_COMMENT;
		else if (cur->line[0] == '.')
			validation_error("Header str not .name or .comment", vd->row, 1);
		// Add validation for multi-string name or comment. Opening and closing "

		// check for other whitespace?
		if (cur->line[0] == '\0' && vd->data == 3)
			return ;
		else
		{
			cur = cur->next;
			vd->row++;
		}
	}
	validation_error("Header incomplete, no '.name'/'.comment'", vd->row, 0);
	// if starts with . and not ".name" ".comment"

	// if line starts with anything but .#;\n = instruction time

	// exit validate_header after first found nl after both .name & .comment are found.
}

void		validate_instructions(t_file *cur, t_validator *vd)
{


	
}

void		validator(t_file *file)
{
	t_file			*cur;
	t_validator		*vd;

	if (!(vd = (t_validator*)ft_memalloc(sizeof(t_validator))))
		handle_error("Malloc failed");
	vd->col = 0;
	vd->row = 0;
	cur = file;

	// validate header
	while (cur)
	{
		validate_header(cur, vd);
		++vd->row;
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
