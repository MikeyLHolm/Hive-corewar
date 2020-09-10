/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 12:41:59 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/10 13:29:20 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "../libft/includes/libft.h"
# include "fcntl.h"
# include "op.h"
# include "asm_op.h"

# define HEADER_NAME		1
# define HEADER_COMMENT		2

typedef struct		s_label
{
	char			*label_name;
	struct s_label	*next;
}					t_label;

typedef struct		s_validator
{
	int				col;
	int				data;
	int				row;
	t_label			*label;
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
**	Tokenization
*/

char		*get_token_label(char *line);

/*
**	Validation and error handling.
*/

void		handle_error(char *message);
t_file		*increment_validator(t_file *cur, t_validator *vd);
void		validation_error(char *message, int row, int col);
void		validator(t_file *file);

/*
**	Misc
*/

void		display_list(t_label *head);

#endif
