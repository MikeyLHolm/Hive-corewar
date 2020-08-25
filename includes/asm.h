/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 12:41:59 by mlindhol          #+#    #+#             */
/*   Updated: 2020/08/25 17:40:36 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "../libft/includes/libft.h"
# include "fcntl.h"
# include "op.h"

extern t_op			g_op_tab[17];

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
}					t_asm;

#endif
