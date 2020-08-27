/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 12:58:05 by mlindhol          #+#    #+#             */
/*   Updated: 2020/08/27 16:33:14 by sadawi           ###   ########.fr       */
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

# define BUF_SIZE		4096

typedef struct 			s_player
{
	char				*filename;
	char				*code;
	char				*comment;
	char				*name;
	int					id;
	int					n;
	int					size;
	struct s_player		*next;
}						t_player;

typedef struct 			s_input
{
	char				*line;
	struct s_input		*next;
}						t_input;

typedef struct 			s_vm
{
	int					flags;
	int					player_n;
	int					players;
	t_player			*player;
	t_input				*file;
}						t_vm;

#endif
