/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_args_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 12:42:42 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 12:49:34 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Validates amount of args statement has.
*/

void		right_n_args(char *statement, int args, int row)
{
	int			i;

	i = -1;
	while (++i < OP_CODE_AMOUNT)
	{
		if (ft_strequ(g_op_tab[i].op_name, statement))
		{
			if (g_op_tab[i].args_n == args)
				return ;
		}
	}
	validation_error("Statement has wrong n of args", row);
}

/*
**	check if last char of line before null and whitespaces is comma
*/

void		trailing_comma(char *line, int row)
{
	int			i;

	i = 0;
	while (line[i])
		++i;
	--i;
	while (ft_isspace(line[i]))
		--i;
	if (line[i] == ',')
		validation_error("Trailing comma in line", row);
}

/*
**	Check that value of argument is either space or digit
*/

int			arg_traverse_value(char *line, int i, int row, int type)
{
	if (ft_isdigit(line[i]) ||
	(line[i] == '-' && ft_isdigit(line[i + 1]) && !(ft_isdigit(line[i - 1]))))
		return (1);
	else if (!ft_isspace(line[i]) ||
			(ft_isspace(line[i]) && line[i + 1] && !ft_isspace(line[i + 1])))
	{
		if (type == REG_CODE)
			validation_error("Extra chars in registry value", row);
		else if (type == DIR_CODE)
			validation_error("Extra chars in direct value", row);
		else
			validation_error("Extra chars in indirect value", row);
	}
	return (0);
}

/*
**	Check that value of argument is either space or one of LABEL_CHARS
*/

int			arg_traverse_label(char *line, int i, int row, int type)
{
	if (ft_strchr(LABEL_CHARS, line[i]))
		return (1);
	else if (!ft_isspace(line[i]))
	{
		if (type == DIR_CODE)
			validation_error("Extra chars in direct value label", row);
		else
			validation_error("Extra chars in indirect value label", row);
	}
	return (0);
}
