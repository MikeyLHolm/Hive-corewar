/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 12:58:05 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/01 13:15:02 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

# include "../libft/includes/libft.h"
# include "fcntl.h"
# include "op.h"

/*
**	Define flags
*/

# define DUMP			1
# define ERROR			2
# define LEAKS			4
# define N				8
# define VISUALIZER		16

# define BUFFER_SIZE	4096

typedef struct			s_player
{
	char				*filename;
	unsigned char		*code;
	char				*comment;
	char				*name;
	int					id;
	int					n;
	int					size;
	struct s_player		*next;
}						t_player;

typedef struct			s_input
{
	char				*line;
	struct s_input		*next;
}						t_input;

typedef struct			s_carriage
{
	int					id;
	int					carry;
	int					reg[REG_NUMBER];
	int					statement;
	int					cycles_left;
	struct s_carriage	*next;
}						t_carriage;

typedef struct			s_vm
{
	int					flags;
	int					player_amount;
	t_player			*tail;
	t_player			*players;
	t_input				*file;
	unsigned char		arena[MEM_SIZE];
	int					player_last_alive;
	int					cycles_to_die;
	int					cycles;
	t_carriage			*carriages;
}						t_vm;

/*
**	Error management functions.
*/

void					handle_error(char *message);

/*
**	Input parsing functions.
*/

void					parse_input(t_vm *vm, int argc, char **argv);
t_player				*save_player(t_vm *vm, char *filename, char *n);

#endif
