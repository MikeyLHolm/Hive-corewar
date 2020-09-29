/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 18:03:47 by sadawi            #+#    #+#             */
/*   Updated: 2020/09/29 10:37:40 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static int		validate_filename(char *filename, char *extension)
{
	if (!(ft_strequ(ft_strrchr(filename, '.'), extension)))
		handle_error("File extension not .s");
	return (1);
}

static t_asm	*init_assm(void)
{
	t_asm		*assm;

	if (!(assm = (t_asm*)ft_memalloc(sizeof(t_asm))))
		handle_error("Malloc failed");
	return (assm);
}

int				main(int argc, char **argv)
{
	t_asm		*assm;

	if (argc != 2 || !validate_filename(argv[1], ".s"))
		handle_error("./asm [filename.s]");
	assm = init_assm();
	assm->file = read_file(argv[1]);
	validator(assm->file);
	tokenize_file(assm);
	convert_labels(assm);
	handle_writing(assm, argv[1]);
	free_assm(assm);
	//system("leaks asm");
	//ft_printf("Conversion successful!\n");
	exit(0);
	return (0);
}
