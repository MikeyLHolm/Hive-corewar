/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 12:44:33 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/15 11:05:32 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Main validator function. Does it in  3 main parts:
**		validate header
**		save labels
**		validate instructions
*/

void		validator(t_file *file)
{
	t_file			*cur;
	t_validator		*vd;
	int				row;

	if (!(vd = (t_validator*)ft_memalloc(sizeof(t_validator))))
		handle_error("Malloc failed");
	vd->row = 1;
	cur = file;
	//display_filelist(cur);
	cur = validate_header(cur, vd);
	cur = increment_validator(cur, vd);
	row = vd->row;
	vd->label = save_labels(cur, vd);
	vd->row = row;
	validate_instructions(cur, vd);
	// free cur and vd->label
	free(vd);
}
