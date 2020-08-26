/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 13:26:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/08/26 15:36:37 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_vm		*init_vm(void)
{
	t_vm	*vm;

	if (!(vm = (t_vm*)ft_memalloc(sizeof(t_vm))))
		handle_error("Malloc failed at VM init");
	vm->flags = 0;
	vm->players = 0;
	return (vm);
}

// void		save_flag(t_vm *vm, char flag)
// {
// 	if (flag == 'd')
// 		vm->flags = vm->flags | DUMP;
// 	else if (flag == 'e')
// 		vm->flags = vm->flags | ERROR;
// 	else if (flag == 'l')
// 		vm->flags = vm->flags | LEAKS;
// 	else if (flag == 'n')
// 		vm->flags = vm->flags | N;
// 	else if (flag == 'v')
// 		vm->flags = vm->flags | VISUALIZER;
// }

void		save_player(t_vm *vm, char **argv)
{
	



}

void		parse_input(t_vm *vm, int argc, char **argv)
{
	int			i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-dump"))
			vm->flags = vm->flags | DUMP;
		else if (ft_strequ(argv[i], "-e"))
			vm->flags = vm->flags | ERROR;
		else if (ft_strequ(argv[i], "-x"))
			vm->flags = vm->flags | LEAKS;
		else if (ft_strequ(argv[i], "-n"))
		{
			vm->flags = vm->flags | N;

		}
		else if (ft_strequ(argv[i], "-v"))
			vm->flags = vm->flags | VISUALIZER;
		else
			save_player(vm, argv[i]);

	}
}

int			main(int argc, char **argv)
{
	t_vm	*vm;

	if (argc < 2)
		handle_error("./vm [filename.cor] ([filename.cor] [filename.cor])");
	vm = init_vm();
	parse_input(vm, argc, argv);
	//read_input();
	//validate();
	//init_arena();

	//introduce_contestants();
	//fight();



	return (0);
}