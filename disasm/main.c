/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 15:15:41 by elindber          #+#    #+#             */
/*   Updated: 2020/09/09 17:41:06 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
# include "disasm.h"
# include "../libft/includes/libft.h"
# include "../includes/asm_op.h"
# include "../includes/op.h"

int		validate_file(int input, t_command *cmnd)
{
	int				i;
	int				name_len;
	int				comment_len;
	unsigned char	code;

	i = -1;
	name_len = 0;
	comment_len = 0;
	while (++i < PROG_NAME_LENGTH + 4)
	{
		read(input, &code, 1);
		if (ft_isprint((int)code) || (int)code == '\n' || (int)code == '\t')
			name_len++;
	}
	i = -1;
	while (++i < COMMENT_LENGTH + 4)
	{
		read(input, &code, 1);
		if (ft_isprint((int)code) || (int)code == '\n' || (int)code == '\t')
			comment_len++;
	}
	i = 0;
	while (read(input, &code, 1))
		i++;
	cmnd->champion_size = i - 4;
	ft_printf("code length: %d\n", i - 4);
	close(input);
	return (name_len <= PROG_NAME_LENGTH
	&& comment_len <= COMMENT_LENGTH && i - 4 <= CHAMP_MAX_SIZE);
}

int		check_header(int fd)
{
	int				i;
	int				magic_header[] = {0x00, 0xea, 0x83, 0xf3};
	int				sign;
	unsigned char	code;
	
	i = 0;
	while (i < 4)
	{
		read(fd, &code, 1);
		sign = 0x0 + (int)code;
		if (sign != magic_header[i])
			return (0);
		i++;
	}
	return (1);
}

void	write_name(int input, int output)
{
	unsigned char	code;
	int				i;
	int				len;

	i = 0;
	len = 0;
	write(output, ".name \"", 7);
	while (i < PROG_NAME_LENGTH + 4)
	{
		read(input, &code, 1);
		if (ft_isprint((int)code))
		{
			write(output, &code, 1);
			len++;
		}
		i++;
	}
//	ft_printf("name len %d %d\n", len, i);
	while (i++ < PROG_NAME_LENGTH + 8)
		read(input, &code, 1);
	write(output, "\"\n", 2);
}

void	write_comment(int input, int output)
{
	unsigned char 	code;
	int				i;
	int				len;

	i = 0;
	len = 0;
	write (output, ".comment \"", 10);
	while (i < COMMENT_LENGTH + 4)
	{
		read(input, &code, 1);
		if (ft_isprint((int)code))
		{
			write(output, &code, 1);
			len++;
		}
		i++;
	}
	write(output, "\"\n\n", 3);
	read(input, &code, 4);
}

void	write_statement(int input, int output, t_command *cmnd)
{
	unsigned char	byte;
	int				tab;

	tab = 0;
	while (tab == 0)
	{
		read(input, &byte, 1);
		tab = (int)byte;
		cmnd->bytes_read++;
	}
	cmnd->statement = tab;
	tab--;
	if (tab >= 0 && tab <= 15)
	{
		write(output, g_op_tab[tab].op_name, ft_strlen(g_op_tab[tab].op_name));
		write(output, " ", 1);
	}
}

void		get_arg_types(int input, int output, t_command *cmnd)
{
	int				i;
	int				bit;
	unsigned char	byte;
	int				value;

	i = 0;
	bit = 7;
	read(input, &byte, 1);
	cmnd->bytes_read++;
	value = (int)byte;
	while (i < 3)
	{
		if (((value >> bit) & 0x01) && ((value >> (bit - 1)) & 0x01))
			cmnd->arg_types[i] = IND_CODE;
		else if (!((value >> bit) & 0x01) && ((value >> (bit - 1)) & 0x01))
			cmnd->arg_types[i] = REG_CODE;
		else if (((value >> bit) & 0x01) && !((value >> (bit - 1)) & 0x01))
			cmnd->arg_types[i] = DIR_CODE;
		else if (!((value >> bit) & 0x01) && !((value >> (bit - 1)) & 0x01))
			cmnd->arg_types[i] = 0;
		bit -= 2;
		i++;
	}
//	printf("values [%d][%d][%d]\n", cmnd->arg_types[0], cmnd->arg_types[1], cmnd->arg_types[2]);
}

int		get_dir_argument_size(t_command *cmnd)
{
	if ((cmnd->statement >= 9 && cmnd->statement <= 12) || cmnd->statement == 14
	|| cmnd->statement == 15)
		return (2);
	else
		return (4);
}

void	write_argument_value(int size, t_command *cmnd)
{
	
}

void	write_arg_sign(int output, int i, t_command *cmnd)
{
	if (cmnd->arg_types[i] == REG_CODE)
		write(output, "r", 1);
	else if (cmnd->arg_types[i] == DIR_CODE)
		write(output, "%", 1);
}

void	write_value(int input, int output, int size)
{
	unsigned char		value;
	int					result;
	int					i;
	int					power;
	char				*str;

	value = 0;
	result = 0;
	i = 1;
	while (i < size)
	{
		read(input, &value, 1);
		power = size - i;
		result += value * (power * 256 );         
		i++;
	}
	read(input, &value, 1);
	result += value;
	result = size == 2 ? (short)result : result;
//	result = size == 2 && result < 0 ? result + 256 : result;
	str = ft_itoa(result);
	write(output, str, ft_strlen(str));
	free(str);
}

void	write_arguments(int input, int output, t_command *cmnd)
{
	int				i;
	int				size;
	char			*str;

	i = 0;
	while (i < 3)
	{
		if (cmnd->arg_types[i] == 0)
			break ;
		else if (cmnd->arg_types[i] == REG_CODE)
			size = 1;
		else if (cmnd->arg_types[i] == IND_CODE)
			size = 2;
		else if (cmnd->arg_types[i] == DIR_CODE)
			size = get_dir_argument_size(cmnd);
		cmnd->bytes_read += size;
		write_arg_sign(output, i, cmnd);
		write_value(input, output, size);
		if (i != 2 && cmnd->arg_types[i + 1] != 0)
			write(output, ", ", 2);
		i++;
	}
}

void	write_instructions(int input, int output, t_command *cmnd)
{
	unsigned char	byte;
	int				i;
	int				len;
	int				*arg_types;
	
	i = 0;
	while (cmnd->bytes_read < cmnd->champion_size)
	{
		write_statement(input, output, cmnd);
		if (cmnd->statement != LIVE && cmnd->statement != ZJMP
		&& cmnd->statement != FORK && cmnd->statement != LFORK)
			get_arg_types(input, output, cmnd);
		else
		{
			cmnd->arg_types[0] = 2;
			cmnd->arg_types[1] = 0;
			cmnd->arg_types[2] = 0;
		}
		write_arguments(input, output, cmnd);
		write(output, "\n", 1);
		i++;
	//	ft_printf("\nbytes read %d\n", cmnd->bytes_read);
	}
}

int		main(int ac, char **av)
{
	int				input;
	int				output;
	char			*name;
	t_command		*cmnd;

	unsigned char	*code;
	int				sign;

	input = open(av[1], O_RDONLY);
	if (!(cmnd = (t_command*)malloc(sizeof(t_command))))
		return (0);
	if (!(check_header(input)) || !(validate_file(input, cmnd)))
		return (0); // change to handle_error later
	input = open(av[1], O_RDONLY);
	name = ft_strjoin(av[1], ".EINO");
	output = open(name, O_WRONLY|O_CREAT|O_TRUNC, 0644);
	free(name);
	write_name(input, output);
	write_comment(input, output);
	cmnd->bytes_read = 0;
	write_instructions(input, output, cmnd);
//	while(read(input, &code, 1) > 0)
//	{
//		sign = 0x0 + (int)code;
//		write(output, &sign, 1);
//	}
	close(output);
	close(input);
	return (0);
}