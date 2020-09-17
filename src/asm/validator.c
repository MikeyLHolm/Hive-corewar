/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 12:44:33 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 12:49:00 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Initialize validator
*/

t_validator		*init_validator(void)
{
	t_validator		*vd;

	if (!(vd = (t_validator*)ft_memalloc(sizeof(t_validator))))
		validation_error("Malloc failed at validator init", 0);
	vd->row = 1;
	vd->name = ft_strjoin(NAME_CMD_STRING, " ");
	vd->comment = ft_strjoin(COMMENT_CMD_STRING, " ");
	return (vd);
}

/*
**	Main validator function. Does it in  3 main parts:
**		validate header
**		save labels
**		validate instructions
*/

void			validator(t_file *file)
{
	t_file			*cur;
	t_validator		*vd;
	int				row;

	vd = init_validator();
	cur = file;
	//display_filelist(cur);
	remove_file_comments(file);
	cur = validate_header(cur, vd);
	cur = increment_validator(cur, vd);
	row = vd->row;
	vd->label = save_labels(cur, vd);
	vd->row = row;
	validate_instructions(cur, vd);
	free_validator(vd);
}
