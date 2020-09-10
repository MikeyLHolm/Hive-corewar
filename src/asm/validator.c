/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 12:44:33 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/10 15:32:12 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_label		*new_label(char *labelname)
{
	t_label		*new;

	if (!(new = (t_label*)ft_memalloc(sizeof(t_label))))
		handle_error("Malloc failed while creating new label");
	new->label_name = labelname;
	return (new);
}

/*
**	Comment validation:
**		[x] double name
**		[x] length > PROG_NAME_LENGTH
**		[] bad characters
**		[x] multi-line name
**		[] no quotes
**		[] open quote
*/

t_file		*validate_name(t_file *cur, t_validator *vd)
{
	char	*name;

	if (vd->data & HEADER_NAME)
		validation_error(ft_strjoin("Duplicate ", NAME_CMD_STRING), vd->row, 1);
	vd->data = vd->data | HEADER_NAME;
	name = NULL;
	name = ft_strjoin(name, ft_strchr(cur->line, '"') + 1);
	// is condition below needed?
	if (!name)
		validation_error("Champion name invalid", vd->row, 1);
	while (!ft_strchr(name, '"') && cur)
	{
		cur = increment_validator(cur, vd);
		name = ft_strjoinfree(name, ft_strdup("\n"));
		name = ft_strjoinfree(name, ft_strdup(cur->line));
		if (ft_strchr(name, '"'))
			break ;
	}
	*ft_strrchr(name, '"') = '\0';
	if (ft_strlen(name) > PROG_NAME_LENGTH)
		validation_error("Champion name too long", vd->row, 1);
	free(name);
	return (increment_validator(cur, vd));
}

/*
**	Comment validation:
**		[x] double comment
**		[x] length > COMMENT_LENGTH
**		[] bad characters
**		[] multi-line comment
**		[] no quotes
*/

t_file		*validate_comment(t_file *cur, t_validator *vd)
{
	char		*comment;

	if (vd->data & HEADER_COMMENT)
		validation_error("Duplicate comment", vd->row, 1);
	vd->data = vd->data | HEADER_COMMENT;
	comment = NULL;
	comment = ft_strjoin(comment, ft_strchr(cur->line, '"') + 1);
	// is condition below needed?
	if (!comment)
		validation_error("Champion comment invalid", vd->row, 1);
	while (!ft_strchr(comment, '"') && cur)
	{
		cur = increment_validator(cur, vd);
		comment = ft_strjoinfree(comment, ft_strdup("\n"));
		comment = ft_strjoinfree(comment, ft_strdup(cur->line));
		if (ft_strchr(comment, '"'))
			break ;
	}
	*ft_strrchr(comment, '"') = '\0';
	if (ft_strlen(comment) > COMMENT_LENGTH)
		validation_error("Champion comment too long", vd->row, 1);
	free(comment);
	return (increment_validator(cur, vd));
}

/*
**	[] strjoin newline if no closing ".
**	[] if starts with . and not ".name" ".comment"
**	[] if line starts with anything but .#;\n = instruction time
**	[] check for other whitespaces?
**	[] exit validate_header after first found nl after both .name & .comment are found.
*/

t_file		*validate_header(t_file *cur, t_validator *vd)
{
	while (cur)
	{
		if (!ft_strncmp(cur->line, ft_strjoin(NAME_CMD_STRING, " "),
			ft_strlen(NAME_CMD_STRING) + 1))
			cur = validate_name(cur, vd);
		else if (!ft_strncmp(cur->line, ft_strjoin(COMMENT_CMD_STRING, " "),
			ft_strlen(COMMENT_CMD_STRING) + 1))
			cur = validate_comment(cur, vd);
		else if (cur->line[0] == '.')
			validation_error("Str not NAME/COMMENT_CMD_STRING", vd->row, 1);
		if (cur->line[0] == '\0' && vd->data == 3)
			return (cur);
		else if (cur->line[0] != '\0' && cur->line[0] != '.' &&
			cur->line[0] != ALT_COMMENT_CHAR && cur->line[0] != COMMENT_CHAR)
			validation_error("Bad string in header", vd->row, 1);
		else if (cur->line[0] == '\0' || cur->line[0] == ALT_COMMENT_CHAR ||
			cur->line[0] == COMMENT_CHAR)
			cur = increment_validator(cur, vd);
	}
	validation_error("Header incomplete, no '.name'/'.comment'", vd->row, 1);
	return (cur);
}

/*
**	Save labels to verify right label is called?
**	Parse all labels first to be able to validate whole statements?
*/

// void		validate_statement()
// {
// }

char		*get_instruction(char *line)
{
	int i;
	int j;
	int len;

	i = 0;
	while (ft_strchr(LABEL_CHARS, line[i]))
		i++;
	if (line[i] == LABEL_CHAR)
		i++;
	len = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	j = i;
	while (line[j])
	{
		if (ft_isalpha(line[j++]))
			len++;
		else
			break ;
	}
	if (len)
		return (ft_strsub(line, i, len));
	return (NULL);
}

void		validate_instruction(char *instruction, int row, t_label *labels)
{
	int			i;
	t_label		*cur;

	i = -1;
	cur = labels;
	while (++i < OP_CODE_AMOUNT)
	{
		ft_printf("op tab [%s] || instr [%s]\n", g_op_tab[i].op_name, instruction);
		if (ft_strequ(g_op_tab[i].op_name, instruction))
		{
			// while (cur)
			// {
			// 	ft_printf("label name [%s]\n", cur->label_name);
			// 	if (ft_strequ(cur->label_name, instruction))
			// 		return ;
			// 	cur = cur->next;
			// }
			return ;
		}
	}
	validation_error("Not valid operation", row, 1);
}

void		validate_instructions(t_file *cur, t_validator *vd,t_label *labels)
{
	char 		*instruction;
	//int			instr_i;

	while (cur)
	{
		instruction = get_instruction(cur->line);
		if (instruction)
			validate_instruction(instruction, vd->row, labels);
		// instr_i = get_instruction_index(instruction);
		// get arguments
		// validate arguments
		// comments at end or invalid chars
		cur = increment_validator(cur, vd);
	}
}

/*
**	Save labels before validating instructions.
**	Since labels can be called before they appear in instructions.
*/

t_label		*save_labels(t_file *head, t_validator *vd)
{
	t_file		*cur;
	t_label		*label;

	cur = head;
	while (cur)
	{
		if (ft_strchr(LABEL_CHARS, cur->line[0]) && cur->line[0] &&
		cur->line[0] != ALT_COMMENT_CHAR && cur->line[0] != COMMENT_CHAR)
		{
			if (!vd->label)
			{
				label = new_label(get_token_label(cur->line));
				vd->label = label;
			}
			else
			{
				label->next = new_label(get_token_label(cur->line));
				label = label->next;
			}
			if (!label->label_name)
				validation_error("Not a valid label", vd->row, 1);
		}
		cur = increment_validator(cur, vd);
	}
	return (vd->label);
}

/*
**	Main validator function. Does it in  3 main parts:
**		validate header
**		save labels
**		validate instructions
*/

void		validator(t_file *file)
{
	t_file			*cur;
	t_validator		*vd;
	int				row;

	if (!(vd = (t_validator*)ft_memalloc(sizeof(t_validator))))
		handle_error("Malloc failed");
	vd->col = 1;
	vd->row = 1;
	cur = file;
	cur = validate_header(cur, vd);
	cur = increment_validator(cur, vd);
	ft_putendl("Header validated, moving to saving labels!\n");
	ft_printf("Post header LINE:: [%s]\n", cur->line);
	row = vd->row;
	vd->label = save_labels(cur, vd);
	display_list(vd->label);
	ft_putendl("Labels saved, moving to instructions!\n");
	vd->row = row;
	ft_printf("Post header LINE:: [%s]\n", cur->line);
	validate_instructions(cur, vd, vd->label);
	free(vd);
}
