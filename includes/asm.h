/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 12:41:59 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 11:19:31 by elindber         ###   ########.fr       */
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
	t_label			*label;
}					t_validator;

typedef struct		s_token
{
	char			**label;
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

int				arg_traverse_label(char *line, int i, int row, int type);
int				arg_traverse_value(char *line, int i, int row, int type);
void			handle_error(char *message);
t_file			*increment_validator(t_file *cur, t_validator *vd);
void			right_n_args(char *statement, int args, int row);
void			trailing_comma(char *line, int row);
void			validate_arg(char *line, char *statement, t_validator *vd, int arg_i);
t_file			*validate_header(t_file *cur, t_validator *vd);
void			validate_instructions(t_file *cur, t_validator *vd);
void			validate_label(char *label, int row, t_label *head);
void			validation_error(char *message, int row);

/*
**	Free memory
*/

void			free_assm(t_asm *assm);
void			free_validator(t_validator *vd);

/*
**	Getters
*/

int				get_arg_type(char *arg);
char			get_argument_type_code(t_token *token);
int				get_first_arg_index(char *line, char *instruction);
int				get_instruction_index(char *instruction);
void			get_name_and_comment(t_asm *assm);
char			*get_output_filename(char *input_filename);

void			get_token_arguments(t_asm *assm, t_token *token);
char			*get_token_instruction(t_asm *assm, int len);
char			**get_token_labels(t_asm *assm);
char			*get_token_label(char *line);
int				get_token_size(t_token *token);

/*
**	Printers
*/

void			print_list(t_label *head);
void			print_filelist(t_file *head);
void			print_file(t_file *head);
void			print_tokens(t_token *token);
void			print_token_info(t_token *token);

/*
**	Prototypes for main
*/

void			convert_labels(t_asm *assm);
t_file			*read_file(char *filename);
void			tokenize_file(t_asm *assm);
void			validator(t_file *file);

/*
**	Utils
*/

int				ft_isspace(int c);
void			handle_error(char *message);
int				line_contains_instruction(t_file *cur);
void			remove_file_comments(t_file *file);
t_label			*save_labels(t_file *head, t_validator *vd);
int				skip_label(char *line);
int				ft_atoi_edgecase(char *str);
int				ft_atoi_edgecase_positive(char *str);
int				ft_atoi_edgecase_negative(char *str);

/*
**	Writers
*/

void			handle_writing(t_asm *assm, char *input_filename);
void			write_argument_type_code(t_token *token, int fd);
void			write_arguments(t_token *token, int fd);
void			write_comment(t_asm *assm, int fd);
void			write_header(int fd);
void			write_name(t_asm *assm, int fd);

#endif
