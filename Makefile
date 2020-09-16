# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elindber <elindber@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/10 19:26:38 by sadawi            #+#    #+#              #
#    Updated: 2020/09/16 12:46:35 by elindber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_ASM = asm
NAME_CW = corewar
NAME_DISASM = disasm

CFILES_ASM = main.c \
			validator.c validator_args.c validator_args_utils.c \
			validator_header.c validator_instructions.c validator_label.c \
			validator_utils.c

SRCS_ASM = $(addprefix src/asm/, $(CFILES_ASM))
OBJS_ASM = $(addprefix objs/, $(notdir $(SRCS_ASM:.c=.o)))

CFILES_CW = main.c statements.c \
			input_parse.c input_sort.c input_utils.c validate_players.c
SRCS_CW = $(addprefix src/vm/, $(CFILES_CW))
OBJS_CW = $(addprefix objs/, $(notdir $(SRCS_CW:.c=.o)))

CFILES_DISASM = main.c validate_file.c write_arguments.c write_statements.c
SRCS_DISASM = $(addprefix src/disasm/, $(CFILES_DISASM))
OBJS_DISASM = $(addprefix objs/, $(notdir $(SRCS_DISASM:.c=.o)))

INCLUDES = -I libft/includes -I includes
FLAGS = -Wall -Wextra -Werror -g
RUN_LIB = make --no-print-directory -C libft/

all:
	@$(RUN_LIB)
	@make --no-print-director $(NAME_ASM) $(NAME_CW) $(NAME_DISASM)

$(NAME_ASM): $(SRCS_ASM) libft/
	@rm -rf objs
	@echo Compiling $(NAME_ASM)...
	@gcc $(FLAGS) $(INCLUDES) -c $(SRCS_ASM)
	@mkdir objs
	@mv $(notdir $(SRCS_ASM:.c=.o)) objs
	@gcc $(FLAGS) $(INCLUDES) -o $(NAME_ASM) $(OBJS_ASM) $(LIBS) libft/libft.a
	@echo $(NAME_ASM) compiled succesfully!

$(NAME_CW): $(SRCS_CW) libft/
	@rm -rf objs
	@echo Compiling $(NAME_CW)...
	@gcc $(FLAGS) $(INCLUDES) -c $(SRCS_CW)
	@mkdir objs
	@mv $(notdir $(SRCS_CW:.c=.o)) objs
	@gcc $(FLAGS) $(INCLUDES) -o $(NAME_CW) $(OBJS_CW) $(LIBS) libft/libft.a -lncurses
	@echo $(NAME_CW) compiled succesfully!

$(NAME_DISASM): $(SRCS_DISASM)
	@rm -rf objs
	@echo Compiling $(NAME_DISASM)...
	@gcc $(FLAGS) $(INCLUDES) -c $(SRCS_DISASM)
	@mkdir objs
	@mv $(notdir $(SRCS_DISASM:.c=.o)) objs
	@gcc $(FLAGS) $(INCLUDES) -o $(NAME_DISASM) $(OBJS_DISASM) $(LIBS) libft/libft.a
	@echo $(NAME_DISASM) compiled succesfully!

lib:
	@$(RUN_LIB)

noflags:
	@rm -rf objs
	@echo Compiling $(NAME_ASM) without flags...
	@gcc $(INCLUDES) -c $(SRCS_ASM)
	@mkdir objs
	@mv $(notdir $(SRCS_ASM:.c=.o)) objs
	@gcc $(INCLUDES) -o $(NAME_ASM) $(OBJS_ASM) libft/libft.a $(LIBS)
	@echo $(NAME_ASM) compiled without flags succesfully!

clean:
	@/bin/rm -f $(OBJS_ASM) $(OBJS_CW) $(OBJS_DISASM)
	@rm -rf objs
	@make -C libft/ clean
	@echo Clean successful!

fclean: clean
	@/bin/rm -f $(NAME_ASM) $(NAME_CW) $(NAME_DISASM)
	@make -C libft/ fclean
	@echo FClean successful!

re: fclean all
