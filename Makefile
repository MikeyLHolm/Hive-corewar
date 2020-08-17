# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/08/17 12:35:54 by mlindhol          #+#    #+#              #
#    Updated: 2020/08/17 12:38:55 by mlindhol         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ASM_NAME = asm
CW_NAME = corewar

CC = gcc
FLAGS = -Wall -Wextra -Werror
LIBRARIES = -lft -L$(LIBFT_DIRECTORY)
INCLUDES = -I$(HEADERS_DIRECTORY) -I$(LIBFT_HEADERS)

LIBFT_DIRECTORY = ./libft/
LIBFT = $(LIBFT_DIRECTORY)libft.a
LIBFT_HEADERS = $(LIBFT_DIRECTORY)includes/

HEADERS_DIRECTORY = ./includes/
HEADER_FILES = push_swap.h
HEADERS = $(addprefix $(HEADERS_DIRECTORY), $(HEADER_FILES))

SOURCES_DIRECTORY = ./src/
SOURCE_FILES =	*.c \
SOURCE_FILES_CH = checker.c
SOURCE_FILES_PS = push_swap.c
SOURCES = $(addprefix $(SOURCES_DIRECTORY), $(SOURCE_FILES))
SOURCES_PS = $(addprefix $(SOURCES_DIRECTORY), $(SOURCE_FILES_PS))
SOURCES_CH = $(addprefix $(SOURCES_DIRECTORY), $(SOURCE_FILES_CH))

OBJECTS_DIRECTORY = ./obj/
OBJECT_FILES = $(patsubst %.c, %.o, $(SOURCE_FILES))
OBJECT_FILES_CH = $(patsubst %.c, %.o, $(SOURCE_FILES_CH))
OBJECT_FILES_PS = $(patsubst %.c, %.o, $(SOURCE_FILES_PS))
OBJECTS	= $(addprefix $(OBJECTS_DIRECTORY), $(OBJECT_FILES))
OBJECTS_PS = $(addprefix $(OBJECTS_DIRECTORY), $(OBJECT_FILES_PS))
OBJECTS_CH = $(addprefix $(OBJECTS_DIRECTORY), $(OBJECT_FILES_CH))

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

all: $(ASM_NAME) $(CW_NAME)

$(CW_NAME): $(LIBFT) $(OBJECTS_DIRECTORY) $(OBJECTS) $(OBJECTS_PS)
	@$(CC) $(FLAGS) $(LIBRARIES) $(INCLUDES) $(OBJECTS) $(OBJECTS_PS) -o $(CW_NAME)
	@echo "\n$(CW_NAME): $(GREEN)$(CW_NAME) object files were created$(RESET)"
	@echo "$(CW_NAME): $(GREEN)$(CW_NAME) was created$(RESET)"

$(ASM_NAME): $(LIBFT) $(OBJECTS_DIRECTORY) $(OBJECTS) $(OBJECTS_CH)
	@$(CC) $(FLAGS) $(LIBRARIES) $(INCLUDES) $(OBJECTS) $(OBJECTS_CH) -o $(ASM_NAME)
	@echo "\n$(CW_NAME): $(GREEN)$(ASM_NAME) object files were created$(RESET)"
	@echo "$(CW_NAME): $(GREEN)$(ASM_NAME) was created$(RESET)"

$(OBJECTS_DIRECTORY):
	@mkdir -p $(OBJECTS_DIRECTORY)
	@echo "$(CW_NAME): $(GREEN)$(OBJECTS_DIRECTORY) was created$(RESET)"

$(OBJECTS_DIRECTORY)%.o : $(SOURCES_DIRECTORY)%.c $(HEADERS)
	@$(CC) $(FLAGS) -c $(INCLUDES) $< -o $@
	@echo "$(GREEN).$(RESET)\c"

$(LIBFT):
	@echo "$(CW_NAME): $(GREEN)creating $(LIBFT)...$(RESET)"
	@$(MAKE) -sC $(LIBFT_DIRECTORY)
	@echo "$(CW_NAME): $(GREEN)$(LIBFT) OK!$(RESET)"

lib:
	@echo "$(CW_NAME): $(GREEN)creating $(LIBFT)...$(RESET)"
	@$(MAKE) -sC $(LIBFT_DIRECTORY)
	@echo "$(CW_NAME): $(GREEN)$(LIBFT) OK!$(RESET)"
run:
	$(CC) $(FLAGS) $(LIBRARIES) $(SOURCES_PS) $(SOURCES)
	./a.out 0 -1 99 17 8 3 55

clean:
	@$(MAKE) -sC $(LIBFT_DIRECTORY) clean
	@rm -rf $(OBJECTS_DIRECTORY)
	@echo "$(CW_NAME): [$(RED)$(OBJECTS_DIRECTORY) was deleted$(RESET)]"
	@echo "$(CW_NAME): [$(RED)object files were deleted$(RESET)]"

fclean: clean
	@rm -f $(LIBFT)
	@echo "$(CW_NAME): [$(RED)$(LIBFT) was deleted$(RESET)]"
	@rm -f $(CW_NAME)
	@echo "$(CW_NAME): [$(RED)$(CW_NAME) was deleted$(RESET)]"
	@rm -f $(ASM_NAME)
	@echo "$(CW_NAME): [$(RED)$(ASM_NAME) was deleted$(RESET)]"

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re