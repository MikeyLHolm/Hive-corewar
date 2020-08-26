/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 18:03:47 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/26 18:25:36 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int		ft_isspace(int c)
{
	return (c == ' ' || c == '\t');
}

void	handle_error(char *message)
{
	ft_printf("%s.\n", message);
	exit(1);
}

t_file	*new_file_link(char *line)
{
	t_file *new;

	if (!(new = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Malloc failed");
	new->line = line;
	return (new);
}

t_file	*read_file(char *filename)
{
	char	*line;
	t_file	*head;
	t_file	*cur;
	int		fd;

	head = NULL;
	if (((fd = open(filename, O_RDONLY)) == -1))
		handle_error("File could not be opened");
	while (get_next_line(fd, &line) > 0)
	{
		if (!head)
		{
			cur = new_file_link(line);
			head = cur;
		}
		else
		{
			cur->next = new_file_link(line);
			cur = cur->next;
		}
	}
	return (head);
}

void	print_file(t_file *head)
{
	t_file *cur;

	cur = head;
	while (cur)
	{
		ft_printf("%s\n", cur->line);
		cur = cur->next;
	}
}

char	*get_output_filename(char *input_filename)
{
	*ft_strrchr(input_filename, '.') = '\0';
	return (ft_strjoin(input_filename, ".cor.sadawi"));
}

void	write_header(int fd)
{
	int		magic_number;
	char 	buf[4];
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
	char 	buf[PROG_NAME_LENGTH];
	size_t	i;

	ft_bzero(buf, PROG_NAME_LENGTH);
	i = 0;
	while (i < ft_strlen(assm->name))
	{
		buf[i] = assm->name[i];
		i++;
	}
	i = 0;
	write(fd, buf, PROG_NAME_LENGTH);
	write(fd, "\0\0\0\0", 4);
}

void	write_comment(t_asm *assm, int fd)
{
	char 	buf[COMMENT_LENGTH];
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

int		get_arg_type(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strchr(arg, '%'))
		return (DIR_CODE);
	if (ft_strchr(arg, 'r') && !ft_strchr(arg, LABEL_CHAR))
		return (REG_CODE);
	return (IND_CODE);
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
		write_direct(token,  token->arg2, fd);
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
	fd = open(output_filename, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	free(output_filename);
	write_header(fd);
	write_name(assm, fd);
	write_exec_code_size_placeholder(assm, fd);
	write_comment(assm, fd);
	write_instructions(assm, fd);
}

t_asm	*init_assm(void)
{
	t_asm *assm;

	if (!(assm = (t_asm*)ft_memalloc(sizeof(t_asm))))
		handle_error("Malloc failed");
	return (assm);
}

char	*get_champion_name(t_file *cur)
{
	char	*name;

	name = NULL;
	name = ft_strjoin(name, ft_strchr(cur->line, '"') + 1);
	if (!name)
		handle_error("Champion name invalid");
	cur = cur->next;
	while (!ft_strchr(name, '"') && cur)
	{
		name = ft_strjoinfree(name, ft_strdup("\n"));
		name = ft_strjoinfree(name, ft_strdup(cur->line));
		if (ft_strchr(name, '"'))
			break ;
		cur = cur->next;
	}
	*ft_strrchr(name, '"') = '\0';
	return (name);
}

char	*get_champion_comment(t_file *cur)
{
	char	*name;

	name = NULL;
	name = ft_strjoin(name, ft_strchr(cur->line, '"') + 1);
	if (!name)
		handle_error("Champion name invalid");
	cur = cur->next;
	while (!ft_strchr(name, '"') && cur)
	{
		name = ft_strjoinfree(name, ft_strdup("\n"));
		name = ft_strjoinfree(name, ft_strdup(cur->line));
		if (ft_strchr(name, '"'))
			break ;
		cur = cur->next;
	}
	*ft_strrchr(name, '"') = '\0';
	return (name);
}

void	get_name_and_comment(t_asm *assm)
{
	t_file *cur;

	cur = assm->file;
	while (cur)
	{
		if (ft_strnequ(cur->line, NAME_CMD_STRING, ft_strlen(NAME_CMD_STRING)))
			assm->name = get_champion_name(cur);
		if (ft_strnequ(cur->line, COMMENT_CMD_STRING, ft_strlen(COMMENT_CMD_STRING)))
			assm->comment = get_champion_comment(cur);
		if (assm->name && assm->comment)
			break ;
		cur = cur->next;
	}
}

char	*get_token_label(char *line)
{
	int i;

	i  = 0;
	while (line[i] && line[i] != LABEL_CHAR)
		if (!ft_strchr(LABEL_CHARS, line[i++]))
			return (NULL);
	i = 0;
	while (line[i] != LABEL_CHAR)
		i++;
	return (ft_strsub(line, 0, i));
}

char	*get_token_instruction(t_asm *assm)
{
	int i;
	int j;
	int len;

	i = 0;
	while (ft_strchr(LABEL_CHARS, assm->cur->line[i]))
		i++;
	if (assm->cur->line[i] == LABEL_CHAR)
		i++;
	len = 0;
	while (!len)
	{
		while (assm->cur->line[i] && ft_isspace(assm->cur->line[i]))
			i++;
		j = i;
		while (assm->cur->line[j])
		{
			if (ft_isalpha(assm->cur->line[j++]))
				len++;
			else
				break;
		}
		if (len)
			return (ft_strsub(assm->cur->line, i, len));
		assm->cur = assm->cur->next;
		i = 0;
		while (!ft_isalpha(assm->cur->line[i]))
			i++;
	}
	return (NULL);
}

int		get_instruction_index(char *instruction)
{
	int i;

	i = 0;
	while (i < OP_CODE_AMOUNT)
	{
		if (ft_strequ(instruction, g_op_tab[i].op_name))
			return (i);
		i++;
	}
	return (-1);
}

int		get_first_arg_index(char *line, char *instruction)
{
	int		i;

	i = 0;
	while (ft_strchr(LABEL_CHARS, line[i]))
		i++;
	if (line[i] == LABEL_CHAR)
		i++;
	while (ft_isspace(line[i]))
		i++;
	i += ft_strlen(instruction);
	while (ft_isspace(line[i]))
		i++;
	return (i);
}

void	get_token_arguments(t_asm *assm, t_token *token)
{
	int		i;
	char	*line;
	char	**args;

	i = 0;
	line = assm->cur->line;
	i = get_first_arg_index(line, token->instruction);
	args = ft_strsplit(&line[i], SEPARATOR_CHAR);
	if (args[0] && args[1] && args[2])
	{
		token->arg3 = ft_strtrim(args[2]);
		free(args[2]);
	}
	if (args[0] && args[1])
	{
		token->arg2 = ft_strtrim(args[1]);
		free(args[1]);
	}
	if (args[0])
	{
		token->arg1 = ft_strtrim(args[0]);
		free(args[0]);
	}
	free(args);
}

void	print_token_info(t_token *token)
{
	ft_printf("LABEL: %s\n", token->label);
	ft_printf("INSTRUCTION: %s\n", token->instruction);
	ft_printf("INSTRUCTION_INDEX: %d\n", token->instruction_index);
	ft_printf("ARG1: %s\n", token->arg1);
	ft_printf("ARG2: %s\n", token->arg2);
	ft_printf("ARG3: %s\n", token->arg3);
	ft_printf("ARGUMENT TYPE CODE: %x\n", token->argument_type_code);
	ft_printf("SIZE: %d\n",  token->size);
	ft_printf("\n\n");
}

char	get_argument_type_code(t_token *token)
{
	int				type;
	unsigned char	byte;

	byte =  0;
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

int		get_token_size(t_token *token)
{
	int type;
	int size;

	size = 1;
	if (g_op_tab[token->instruction_index].args_type_code)
		size += 1;
	type = get_arg_type(token->arg1);
	if (type == REG_CODE)
		size += 1;
	else if (type == IND_CODE)
		size += 2;
	else if (type == DIR_CODE)
		size += (g_op_tab[token->instruction_index].size_t_dir ? 2 : 4);
	type = get_arg_type(token->arg2);
	if (type == REG_CODE)
		size += 1;
	else if (type == IND_CODE)
		size += 2;
	else if (type == DIR_CODE)
		size += (g_op_tab[token->instruction_index].size_t_dir ? 2 : 4);
	type = get_arg_type(token->arg3);
	if (type == REG_CODE)
		size += 1;
	else if (type == IND_CODE)
		size += 2;
	else if (type == DIR_CODE)
		size += (g_op_tab[token->instruction_index].size_t_dir ? 2 : 4);
	return (size);
}

t_token	*new_token(t_asm *assm)
{
	t_token *token;

	if (!(token = (t_token*)ft_memalloc(sizeof(t_token))))
		handle_error("Malloc failed");
	token->label = get_token_label(assm->cur->line);
	token->instruction = get_token_instruction(assm);
	token->instruction_index = get_instruction_index(token->instruction);
	get_token_arguments(assm, token);
	token->argument_type_code = get_argument_type_code(token);
	token->size = get_token_size(token);
	assm->champion_size += token->size;
	//print_token_info(token);
	return (token);
}

int		line_contains_instruction(t_file *cur)
{
	int i;

	i = 0;
	if (!cur->line)
		return (0);
	while (cur->line[i])
	{
		if (cur->line[i] == '.' || cur->line[i] == COMMENT_CHAR || cur->line[i] == ALT_COMMENT_CHAR)
			return (0);
		if (!ft_isspace(cur->line[i++]))
			return (1);
	}
	return (0);
}

void	tokenize_file(t_asm *assm)
{
	t_token	*cur_token;

	get_name_and_comment(assm);
	assm->cur = assm->file;
	while (assm->cur->line && assm->cur->line[0] == '.')
		assm->cur = assm->cur->next;
	assm->cur = assm->cur->next;
	while (assm->cur)
	{
		if (line_contains_instruction(assm->cur))
		{
			if (!assm->token)
			{
				cur_token = new_token(assm);
				assm->token  = cur_token;
			}
			else
			{
				cur_token->next = new_token(assm);
				cur_token = cur_token->next;
			}
		}
		assm->cur = assm->cur->next;
	}
}

void	convert_argument_label(t_asm *assm, char **arg, int arg_position)
{
	t_token	*cur_token;
	int		label_position;
	char	*converted_arg;

	label_position = 0;
	cur_token = assm->token;
	while (cur_token)
	{
		if (ft_strequ(cur_token->label, ft_strchr(*arg, ':') + 1))
		{
			if ((*arg)[0] == '%')
				converted_arg = ft_strjoinfree(ft_strdup("%"), ft_itoa(label_position  - arg_position));
			else
				converted_arg = ft_itoa(label_position  - arg_position);
			free(*arg);
			*arg = converted_arg;
			return ;
		}
		label_position += cur_token->size;
		cur_token = cur_token->next;
	}
}

void	convert_labels(t_asm *assm)
{
	t_token *cur_token;
	int		position;

	position = 0;
	cur_token = assm->token;
	while (cur_token)
	{
		if (cur_token->arg1 && ft_strchr(cur_token->arg1, ':'))
			convert_argument_label(assm, &cur_token->arg1, position);
		if (cur_token->arg2 && ft_strchr(cur_token->arg2, ':'))
			convert_argument_label(assm, &cur_token->arg2, position);
		if (cur_token->arg3 && ft_strchr(cur_token->arg3, ':'))
			convert_argument_label(assm, &cur_token->arg3, position);
		position += cur_token->size;
		cur_token = cur_token->next;
	}
}

void	print_tokens(t_token *token)
{
	t_token *cur;

	cur = token;
	while (cur)
	{
		ft_printf("CONVERTED TOKEN\n");
		print_token_info(cur);
		cur = cur->next;
	}
}

void	remove_file_comments(t_file *file)
{
	t_file *cur;

	cur = file;
	while (cur)
	{
		if (line_contains_instruction(cur))
		{
			if (ft_strchr(cur->line, COMMENT_CHAR))
				*ft_strchr(cur->line, COMMENT_CHAR) = '\0';
			if (ft_strchr(cur->line, ALT_COMMENT_CHAR))
				*ft_strchr(cur->line, ALT_COMMENT_CHAR) = '\0';
		}
		cur = cur->next;
	}
}

int		main(int argc, char **argv)
{
	t_asm	*assm;

	if (argc != 2)
		handle_error("./asm [filename.s]");
	assm = init_assm();
	assm->file = read_file(argv[1]);
	remove_file_comments(assm->file);
	//check_file(assm->file);
	tokenize_file(assm);
	convert_labels(assm);
	print_tokens(assm->token);
	print_file(assm->file);
	int byte;
	byte = -19;
	ft_printf("BYTE: %hx\n", byte);
	ft_printf("%02hx\n", 15);
	handle_writing(assm, argv[1]);
	//system("leaks asm");
	exit(0);
	return (0);
}
