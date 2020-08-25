/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 18:03:47 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/25 15:03:07 by sadawi           ###   ########.fr       */
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
	return (ft_strjoin(input_filename, ".cor"));
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

void	write_exec_code_size_placeholder(int fd)
{
	write(fd, "\0\0\0\0", 4);
}

void	handle_writing(t_asm *assm, char *input_filename)
{
	int		fd;
	char	*output_filename;

	output_filename = get_output_filename(input_filename);
	fd = open(output_filename, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	write_header(fd);
	write_name(assm, fd);
	write_exec_code_size_placeholder(fd);
	write_comment(assm, fd);
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

char	*get_label(char *line)
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

char	*get_instruction(t_asm *assm)
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

void	get_arguments(t_asm *assm, t_token *token)
{
	int		i;
	char	*line;
	char	**args;

	i = 0;
	line = assm->cur->line;
	while (line[i] && line[i] != SEPARATOR_CHAR)
		i++;
	while (i > 1 && !ft_isspace(line[i - 1]))
		i--;
	args = ft_strsplit(&line[i], SEPARATOR_CHAR);
	token->arg1 = (*args ? *args++ : NULL);
	token->arg2 = (*args ? *args++ : NULL);
	token->arg3 = (*args ? *args++ : NULL);
}

t_token	*new_token(t_asm *assm)
{
	t_token *token;

	if (!(token = (t_token*)ft_memalloc(sizeof(t_token))))
		handle_error("Malloc failed");
	token->label = get_label(assm->cur->line);
	ft_printf("LABEL: %s\n", token->label);
	token->instruction =  get_instruction(assm);
	ft_printf("INSTRUCTION: %s\n", token->instruction);
	get_arguments(assm,  token);
	ft_printf("ARG1: %s\n", token->arg1);
	ft_printf("ARG2: %s\n", token->arg2);
	ft_printf("ARG3: %s\n", token->arg3);
	ft_printf("\n\n");
	return (token);
}

int		line_contains_instruction(t_file *cur)
{
	int i;

	i = 0;
	if (!cur->line)
		return (0);
	while (cur->line[i])
		if (!ft_isspace(cur->line[i++]))
			return (1);
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

int		main(int argc, char **argv)
{
	t_asm	*assm;

	if (argc != 2)
		handle_error("./asm [filename.s]");
	assm = init_assm();
	assm->file = read_file(argv[1]);
	//check_file(assm->file);
	tokenize_file(assm);
	print_file(assm->file);
	handle_writing(assm, argv[1]);
	return (0);
}
