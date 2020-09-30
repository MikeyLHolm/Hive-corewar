/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 16:37:58 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 11:18:54 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void			write_argument_type_code(t_token *token, int fd)
{
	unsigned char	buf;

	if (g_op_tab[token->instruction_index].args_type_code)
	{
		buf = token->argument_type_code;
		write(fd, &buf, 1);
	}
}

static void		write_registry(char *arg, int fd)
{
	unsigned char	buf;

	buf = ft_atoi(ft_strchr(arg, 'r') + 1);
	write(fd, &buf, 1);
}

static void		write_direct(t_token *token, char *arg, int fd)
{
	int				bytes;

	bytes = ft_atoi_edgecase(&arg[1]);
	if (!g_op_tab[token->instruction_index].size_t_dir)
	{
		write(fd, &((unsigned char*)&bytes)[3], 1);
		write(fd, &((unsigned char*)&bytes)[2], 1);
	}
	write(fd, &((unsigned char*)&bytes)[1], 1);
	write(fd, &((unsigned char*)&bytes)[0], 1);
}

static void		write_indirect(char *arg, int fd)
{
	short			bytes;

	bytes = ft_atoi_edgecase(arg);
	write(fd, &((unsigned char*)&bytes)[1], 1);
	write(fd, &((unsigned char*)&bytes)[0], 1);
}

void			write_arguments(t_token *token, int fd)
{
	int				type;

	type = get_arg_type(token->arg1);
	if (type == REG_CODE)
		write_registry(token->arg1, fd);
	else if (type == DIR_CODE)
		write_direct(token, token->arg1, fd);
	else if (type == IND_CODE)
		write_indirect(token->arg1, fd);
	type = get_arg_type(token->arg2);
	if (type == REG_CODE)
		write_registry(token->arg2, fd);
	else if (type == DIR_CODE)
		write_direct(token, token->arg2, fd);
	else if (type == IND_CODE)
		write_indirect(token->arg2, fd);
	type = get_arg_type(token->arg3);
	if (type == REG_CODE)
		write_registry(token->arg3, fd);
	else if (type == DIR_CODE)
		write_direct(token, token->arg3, fd);
	else if (type == IND_CODE)
		write_indirect(token->arg3, fd);
}
