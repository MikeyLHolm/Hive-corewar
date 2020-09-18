/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_instructions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 14:51:13 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/18 15:17:29 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Removes comments from the end of last arg
*/

char		*remove_comment(char *arg, int row)
{
	int			i;
	int			j;
	int			len;

	i = 0;
	len = 0;
	while (arg[i] && ft_isspace(arg[i]))
		++i;
	j = i;
	while (arg[i] && !ft_isspace(arg[i]))
	{
		++i;
		++len;
	}
	while (arg[i] && ft_isspace(arg[i]))
		++i;
	if (arg[i] != '\0' && arg[i] != COMMENT_CHAR && arg[i] != ALT_COMMENT_CHAR)
		validation_error("Extra chars at end of args", row);
	return (ft_strsub(arg, j, len));
}

/*
**	Finds statement from line and returns it
*/

char		*get_statement(char *line, int row)
{
	int i;
	int j;
	int len;

	i = 0;
	//ft_printf("[%s]\n", line);
	while (ft_strchr(LABEL_CHARS, line[i]))
		i++;
	i = line[i] == LABEL_CHAR ? i + 1 : 0;
	len = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	j = i - 1;
	while (line[++j])
	{
		if (ft_isalpha(line[j]))
			len++;
		else if (!ft_isspace(line[j]))
			validation_error("No space after statement", row);
		else
			break ;
	}
	if (len)
		return (ft_strsub(line, i, len));
	return (NULL);
}

/*
**	Checks that statement input is an actual statement
*/

void		validate_statement(char *statement, t_validator *vd)
{
	int			i;
	t_label		*cur;

	i = -1;
	cur = vd->label;
	while (++i < OP_CODE_AMOUNT)
	{
		if (ft_strequ(g_op_tab[i].op_name, statement))
			return ;
	}
	validation_error("Not valid operation", vd->row);
}

/*
**	Dispatcher to validate arguments.
*/

void		validate_args(char *line, char *statement, t_validator *vd)
{
	int			i;
	char		**args;

	trailing_comma(line, vd->row);
	i = get_first_arg_index(line, statement);
	//ft_printf("whole line [%s]\n", line);
	args = ft_strsplit(&line[i], SEPARATOR_CHAR);
	i = 0;
	while (args[i])
		i++;
	right_n_args(statement, i--, vd->row);
	while (i >= 0 && args[i])
	{
		validate_arg(ft_strtrim(args[i]), statement, vd, i);
		free(args[i--]);
	}
	free(args);
}

/*
**	Dispatcher for instruction validation.
*/

void		validate_instructions(t_file *cur, t_validator *vd)
{
	char		*statement;
	int			not_empty;

	not_empty = 0;
	while (cur)
	{
		if (cur->line[0])
		{
			not_empty = 1;
			statement = get_statement(cur->line, vd->row);
			if (statement)
			{
				validate_statement(statement, vd);
				validate_args(cur->line, statement, vd);
				free(statement);
			}
		}
		cur = increment_validator(cur, vd);
	}
	if (not_empty == 0)
		validation_error("No instructions!", vd->row);
}
