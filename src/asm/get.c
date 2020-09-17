/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 13:53:49 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 16:14:46 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int			get_first_arg_index(char *line, char *instruction)
{
	int			i;

	i = 0;
	while (ft_strchr(LABEL_CHARS, line[i]))
		i++;
	i = line[i] == LABEL_CHAR ? i + 1 : 0;
	while (ft_isspace(line[i]))
		i++;
	i += ft_strlen(instruction);
	while (ft_isspace(line[i]))
		i++;
	return (i);
}

int			get_arg_type(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strchr(arg, '%'))
		return (DIR_CODE);
	if (ft_strchr(arg, 'r') && !ft_strchr(arg, LABEL_CHAR))
		return (REG_CODE);
	return (IND_CODE);
}

char		get_argument_type_code(t_token *token)
{
	int				type;
	unsigned char	byte;

	byte = 0;
	type = get_arg_type(token->arg1);
	if (type == DIR_CODE || type == IND_CODE)
		byte |= 1UL << 7;
	if (type == REG_CODE || type == IND_CODE)
		byte |= 1UL << 6;
	type = get_arg_type(token->arg2);
	if (type == DIR_CODE || type == IND_CODE)
		byte |= 1UL << 5;
	if (type == REG_CODE || type == IND_CODE)
		byte |= 1UL << 4;
	type = get_arg_type(token->arg3);
	if (type == DIR_CODE || type == IND_CODE)
		byte |= 1UL << 3;
	if (type == REG_CODE || type == IND_CODE)
		byte |= 1UL << 2;
	return (byte);
}

int			get_instruction_index(char *instruction)
{
	int			i;

	i = 0;
	while (i < OP_CODE_AMOUNT)
	{
		if (ft_strequ(instruction, g_op_tab[i].op_name))
			return (i);
		i++;
	}
	return (-1);
}

char		*get_output_filename(char *input_filename)
{
	*ft_strrchr(input_filename, '.') = '\0';
	return (ft_strjoin(input_filename, ".cor"));
}
