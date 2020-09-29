/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_binary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 12:58:57 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 18:51:17 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static void			write_exec_code_size(t_asm *assm, int fd)
{
	write(fd, &((unsigned char*)&assm->champion_size)[3], 1);
	write(fd, &((unsigned char*)&assm->champion_size)[2], 1);
	write(fd, &((unsigned char*)&assm->champion_size)[1], 1);
	write(fd, &((unsigned char*)&assm->champion_size)[0], 1);
}

static void			write_statement(t_token *token, int fd)
{
	unsigned char	buf;

	buf = token->instruction_index + 1;
	write(fd, &buf, 1);
}

static void			write_instructions(t_asm *assm, int fd)
{
	t_token			*token;

	token = assm->token;
	while (token)
	{
		if (token->instruction)
		{
			write_statement(token, fd);
			write_argument_type_code(token, fd);
			write_arguments(token, fd);
		}
		token = token->next;
	}
}

void				handle_writing(t_asm *assm, char *input_filename)
{
	int				fd;
	char			*output_filename;

	output_filename = get_output_filename(input_filename);
	if (!(fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)))
		handle_error("Creating output file failed");
	free(output_filename);
	write_header(fd);
	write_name(assm, fd);
	write_exec_code_size(assm, fd);
	write_comment(assm, fd);
	write_instructions(assm, fd);
}
