# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/10 19:26:38 by sadawi            #+#    #+#              #
#    Updated: 2020/09/07 16:54:37 by mlindhol         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_ASM = asm
NAME_CW = corewar

CFILES_ASM = main.c validator.c validator_utils.c
SRCS_ASM = $(addprefix src/asm/, $(CFILES_ASM))
OBJS_ASM = $(addprefix objs/, $(notdir $(SRCS_ASM:.c=.o)))

CFILES_CW = main.c statements.c parse_input.c sort_players.c validate_input.c
SRCS_CW = $(addprefix src/vm/, $(CFILES_CW))
OBJS_CW = $(addprefix objs/, $(notdir $(SRCS_CW:.c=.o)))

INCLUDES = -I libft/includes -I includes
FLAGS = -Wall -Wextra -Werror -g
RUN_LIB = make --no-print-directory -C libft/

all:
	@$(RUN_LIB)
	@make --no-print-director $(NAME_ASM) $(NAME_CW)

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
	@/bin/rm -f $(OBJS_ASM) $(OBJS_CW)
	@rm -rf objs
	@make -C libft/ clean
	@echo Clean successful!

fclean: clean
	@/bin/rm -f $(NAME_ASM) $(NAME_CW)
	@make -C libft/ fclean
	@echo FClean successful!

re: fclean all
