/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 16:54:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/07 16:57:57 by mlindhol         ###   ########.fr       */
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
