/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disasm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 13:02:27 by elindber          #+#    #+#             */
/*   Updated: 2020/09/16 14:33:16 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISASM_H
# define DISASM_H

# include <unistd.h>
# include <fcntl.h>
# include "../libft/includes/libft.h"
# include "../includes/asm_op.h"
# include "../includes/op.h"

# define LIVE 1
# define LD 2
# define ST 3
# define ADD 4
# define SUB 5
# define AND 6
# define OR 7
# define XOR 8
# define ZJMP 9
# define LDI 10
# define STI 11
# define FORK 12
# define LLD 13
# define LLDI 14
# define LFORK 15
# define AFF 16

typedef struct	s_command
{
	int			size_told;
	int			bytes_read;
	int			statement;
	int			arg_types[3];
	int			champion_size;
	char		*nbrs;
}				t_command;

int				check_header_and_file_name(int fd, char *name);
int				validate_file
(int input, t_command *cmnd, int name_len, int comment_len);
void			write_instructions(int input, int output, t_command *cmnd);
void			write_arguments(int input, int output, t_command *cmnd);

#endif
