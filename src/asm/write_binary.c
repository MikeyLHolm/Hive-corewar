/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_binary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 12:58:57 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 13:01:10 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	write_header(int fd)
{
	int		magic_number;
	char	buf[4];
	int		i;

	magic_number = COREWAR_EXEC_MAGIC;
	i = 0;
	while (i < 4)
	{
		buf[i++] = magic_number % 256;
		magic_number /= 256;
	}
	while (--i >= 0)
		write(fd, &buf[i], 1);
}

void	write_name(t_asm *assm, int fd)
{
	char	buf[PROG_NAME_LENGTH];
	size_t	i;

	ft_bzero(buf, PROG_NAME_LENGTH);
	i = 0;
	while (i < ft_strlen(assm->name))
	{
		buf[i] = assm->name[i];
		i++;
	}
	i = 0;
	write(fd, buf, PROG_NAME_LENGTH + 1);
	write(fd, "\0\0\0\0", 3);
}

void	write_comment(t_asm *assm, int fd)
{
	char	buf[COMMENT_LENGTH];
	size_t	i;

	ft_bzero(buf, COMMENT_LENGTH);
	i = 0;
	while (i < ft_strlen(assm->comment))
	{
		buf[i] = assm->comment[i];
		i++;
	}
	i = 0;
	write(fd, buf, COMMENT_LENGTH);
	write(fd, "\0\0\0\0", 4);
}

void	write_exec_code_size_placeholder(t_asm *assm, int fd)
{
	write(fd, &((unsigned char*)&assm->champion_size)[3], 1);
	write(fd, &((unsigned char*)&assm->champion_size)[2], 1);
	write(fd, &((unsigned char*)&assm->champion_size)[1], 1);
	write(fd, &((unsigned char*)&assm->champion_size)[0], 1);
}

void	write_statement(t_token *token, int fd)
{
	unsigned char	buf;

	buf = token->instruction_index + 1;
	write(fd, &buf, 1);
}

void	write_argument_type_code(t_token *token, int fd)
{
	unsigned char	buf;

	if (g_op_tab[token->instruction_index].args_type_code)
	{
		buf = token->argument_type_code;
		write(fd, &buf, 1);
	}
}

void	write_registry(char *arg, int fd)
{
	unsigned char	buf;

	buf = ft_atoi(ft_strchr(arg, 'r') + 1);
	write(fd, &buf, 1);
}

void	write_direct(t_token *token, char *arg, int fd)
{
	int bytes;

	bytes = ft_atoi(&arg[1]);
	if (!g_op_tab[token->instruction_index].size_t_dir)
	{
		write(fd, &((unsigned char*)&bytes)[3], 1);
		write(fd, &((unsigned char*)&bytes)[2], 1);
	}
	write(fd, &((unsigned char*)&bytes)[1], 1);
	write(fd, &((unsigned char*)&bytes)[0], 1);
}

void	write_indirect(char *arg, int fd)
{
	short bytes;

	bytes = ft_atoi(arg);
	write(fd, &((unsigned char*)&bytes)[1], 1);
	write(fd, &((unsigned char*)&bytes)[0], 1);
}

void	write_arguments(t_token *token, int fd)
{
	int	type;

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

void	write_instructions(t_asm *assm, int fd)
{
	t_token	*token;

	token = assm->token;
	while (token)
	{
		write_statement(token, fd);
		write_argument_type_code(token, fd);
		write_arguments(token, fd);
		token = token->next;
	}
}

void	handle_writing(t_asm *assm, char *input_filename)
{
	int		fd;
	char	*output_filename;

	output_filename = get_output_filename(input_filename);
	//check if open failed here
	fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	free(output_filename);
	write_header(fd);
	write_name(assm, fd);
	write_exec_code_size_placeholder(assm, fd);
	write_comment(assm, fd);
	write_instructions(assm, fd);
}
