/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 14:53:42 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/15 09:37:37 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Returns type of argument as int.
*/

static int		get_arg_type(char *arg)
{
	if (!arg[0])
		return (0);
	if (arg[0] == '%')
		return (T_DIR);
	if (arg[0] == 'r' && !ft_strchr(arg, LABEL_CHAR))
		return (T_REG);
	return (T_IND);
}

/*
**	Test comments and other shitty inputs to args!
*/

void			validate_reg_value(char *line, t_validator *vd)
{
	int				i;
	int				len;
	long			r;

	i = 0;
	len = 0;
	r = -1;
	if (ft_strchr(line, COMMENT_CHAR) || ft_strchr(line, ALT_COMMENT_CHAR))
		validation_error("Comment in wrong place", vd->row);
	while (line[++i])
	{
		if (ft_isdigit(line[i]))
			++len;
		else if (!ft_isspace(line[i]))
			validation_error("Extra chars in registry value", vd->row);
	}
	if (len > 0)
		r = ft_atoilong(ft_strsub(line, 1, len));
	if (len < 1 || r < 0 || r > 2147483647)
		validation_error("Registry not an int", vd->row);
}

void			validate_dir_value(char *line, t_validator *vd)
{
	int				i;
	int				len;
	long			r;

	i = line[1] != LABEL_CHAR ? 0 : 1;
	len = 0;
	r = -1;
	if (ft_strchr(line, COMMENT_CHAR) || ft_strchr(line, ALT_COMMENT_CHAR))
		validation_error("Comment in wrong place", vd->row);
	while (line[++i])
	{
		if (line[1] != LABEL_CHAR)
		{
			if (ft_isdigit(line[i]))
				++len;
			else if (!ft_isspace(line[i]))
				validation_error("Extra chars in instruction", vd->row);
		}
		else
		{
			if (ft_strchr(LABEL_CHARS, line[i]))
				++len;
			else if (!ft_isspace(line[i]))
				validation_error("Extra chars in direct value", vd->row);
		}
	}
	if (len > 0 && line[1] != LABEL_CHAR)
		r = ft_atoilong(ft_strsub(line, 1, len));
	else if (len > 0)
		validate_label(ft_strsub(line, 2, len), vd->row, vd->label);
	if (len < 1 || (line[1] != LABEL_CHAR && (r < 0 || r > 2147483647)))
		validation_error("Direct value not an int or label", vd->row);
}

void			validate_ind_value(char *line, t_validator *vd)
{
	int				i;
	int				len;
	long			r;

	i = line[1] != LABEL_CHAR ? -1 : 0;
	len = 0;
	r = -1;
	if (ft_strchr(line, COMMENT_CHAR) || ft_strchr(line, ALT_COMMENT_CHAR))
		validation_error("Comment in wrong place", vd->row);
	while (line[++i])
	{
		if (line[0] != LABEL_CHAR)
		{
			if (ft_isdigit(line[i]))
				++len;
			else if (!ft_isspace(line[i]))
				validation_error("Extra chars in instruction", vd->row);
		}
		else
		{
			if (ft_strchr(LABEL_CHARS, line[i]))
				++len;
			else if (!ft_isspace(line[i]))
				validation_error("Extra chars in indirect value", vd->row);
		}
	}
	if (len > 0 && line[0] != LABEL_CHAR)
		r = ft_atoilong(ft_strsub(line, 0, len));
	else if (len > 0)
		validate_label(ft_strsub(line, 1, len), vd->row, vd->label);
	if (len < 1 || (line[0] != LABEL_CHAR && (r < 0 || r > 2147483647)))
		validation_error("Direct value not an int or label", vd->row);
}

/*
**	Validates single argument
*/

void			validate_arg(char *line, char *statement,
								t_validator *vd, int arg_i)
{
	int			i;
	int			type;

	i = -1;
	//ft_printf("argument inside [%s]\n", line);
	while (++i < OP_CODE_AMOUNT)
	{
		if (ft_strequ(g_op_tab[i].op_name, statement))
		{
			type = get_arg_type(line);
			if (!(g_op_tab[i].args_type[arg_i] & type))
				validation_error("Wrong arg type for the statement", vd->row);
			if (type == T_REG)
				validate_reg_value(line, vd);
			else if (type == T_DIR)
				validate_dir_value(line, vd);
			else if (type == T_IND)
				validate_ind_value(line, vd);
		}
	}
}
