/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 12:41:59 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/04 12:55:36 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "../libft/includes/libft.h"
# include "fcntl.h"
# include "op.h"
# include "asm_op.h"

typedef struct		s_validator
{
	int				col;
	int				row;
}					t_validator;


typedef struct		s_token
{
	char			*label;
	char			*instruction;
	int				instruction_index;
	char			*arg1;
	char			*arg2;
	char			*arg3;
	unsigned char	argument_type_code;
	int				size;
	struct s_token	*next;
}					t_token;

typedef struct		s_file
{
	char			*line;
	struct s_file	*next;
}					t_file;

typedef struct		s_asm
{
	char			*name;
	char			*comment;
	t_file			*file;
	t_file			*cur;
	t_token			*token;
	int				champion_size;
}					t_asm;

/*
**	Validation and error handling.
*/

void		handle_error(char *message);
void		validator(t_file *file);

#endif
