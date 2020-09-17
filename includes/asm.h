/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 12:41:59 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 12:29:43 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "../libft/includes/libft.h"
# include "op.h"
# include "asm_op.h"
# include <fcntl.h>
# include <limits.h>

# define HEADER_NAME		1
# define HEADER_COMMENT		2

typedef struct		s_label
{
	char			*label_name;
	struct s_label	*next;
}					t_label;

typedef struct		s_validator
{
	int				data;
	int				row;
	char			*name;
	char			*comment;
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

int			get_first_arg_index(char *line, char *instruction);
char		*get_token_label(char *line);

/*
**	Validation and error handling.
*/

void		handle_error(char *message);
t_file		*increment_validator(t_file *cur, t_validator *vd);
void		validation_error(char *message, int row);
void		validator(t_file *file);
void		validate_arg(char *line, char *statement, t_validator *vd, int arg_i);
t_file		*validate_header(t_file *cur, t_validator *vd);
void		validate_instructions(t_file *cur, t_validator *vd);
void		validate_label(char *label, int row, t_label *head);
t_label		*save_labels(t_file *head, t_validator *vd);
void		right_n_args(char *statement, int args, int row);
void		trailing_comma(char *line, int row);
int			arg_traverse_label(char *line, int i, int row, int type);
int			arg_traverse_value(char *line, int i, int row, int type);
void		remove_file_comments(t_file *file);

/*
**	Misc
*/

void		display_list(t_label *head);
void		display_filelist(t_file *head);
int			ft_isspace(int c);

/*
**	Free memory
*/

void		free_assm(t_asm *assm);
void		free_validator(t_validator *vd);

#endif
