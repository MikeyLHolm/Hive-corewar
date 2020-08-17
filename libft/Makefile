# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 09:28:21 by mlindhol          #+#    #+#              #
#    Updated: 2020/03/04 14:08:27 by mlindhol         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = gcc 
FLAGS = -Wall -Wextra -Werror

LIBFT_SRC =	ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
			ft_isdigit.c ft_isprint.c ft_itoa.c	ft_memalloc.c ft_memccpy.c \
			ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c ft_memmove.c \
			ft_memset.c ft_putchar.c ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c \
			ft_putnbr.c ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c ft_strcat.c \
			ft_strchr.c ft_strclr.c ft_strcmp.c ft_strcpy.c ft_strdel.c \
			ft_strdup.c ft_strequ.c ft_striter.c ft_striteri.c ft_strjoin.c \
			ft_strlcat.c ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c \
			ft_strncmp.c ft_strncpy.c ft_strnequ.c ft_strnew.c ft_strnstr.c \
			ft_strrchr.c ft_strsplit.c ft_strstr.c ft_strsub.c ft_strtrim.c \
			ft_tolower.c ft_toupper.c ft_lstnew.c ft_lstdelone.c ft_lstdel.c \
			ft_lstadd.c ft_lstiter.c ft_lstmap.c \
			ft_swap.c ft_islower.c ft_isupper.c ft_abs.c ft_intlen.c \
			ft_isspace.c ft_word_count.c ft_strndup.c ft_itoa_uimax.c \
			ft_intlen_uimax.c ft_itoa_base.c ft_ftoa.c ft_pow.c ft_tolower_str.c \
			ft_toupper_str.c ft_strlen_int.c ft_atoimax.c ft_strsplit_free.c

FT_PRINTF_DIR = ./ft_printf/
FT_PRINTF_SRC_DIR = $(FT_PRINTF_DIR)srcs/
FT_PRINTF_SRC = display_c.c display_d.c display_s.c preparse.c \
		ft_printf.c parsing.c display_p.c utility.c \
		display_pct.c length_conversions.c display_u.c \
		display_o.c display_x.c display_f.c display_b.c
FT_PRINTF_SOURCES = $(addprefix $(FT_PRINTF_SRC_DIR), $(FT_PRINTF_SRC))

GNL_DIR = ./gnl/
GNL_SRC = get_next_line.c

OBJ_DIR = ./objs/
OBJECTS = $(LIBFT_SRC:.c=.o)
OBJECTS += $(FT_PRINTF_SOURCES:.c=.o)
OBJECTS += $(GNL_DIR)$(GNL_SRC:.c=.o)

HEADERS += $(FT_PRINTF_DIR)includes/ft_printf.h
HEADERS += $(GNL_DIR)includes/get_next_line.h
HEADERS += ./includes/libft.h

INCLUDES = -I$(HEADERS)

# FT_PRINTF_OBJ_DIR = ./objs/
# FT_PRINTF_OBJ_FILES = $(FT_PRINTF_SOURCES:.c=.o)
# FT_PRINTF_OBJECTS = $(addprefix $(FT_PRINTF_OBJ_DIR), $(FT_PRINTF_OBJ_FILES))
# GNL_OBJ_DIR = ./objs/
# GNL_OBJ_FILES = $(GNL_DIR)$(GNL_SRC:.c=.o)
# GNL_OBJECTS = = $(GNL_OBJ_DIR)$(GNL_OBJ_FILES))
# SRCS = $(FT_PRINTF_SOURCES) $(GNL_SRC) $(LIBFT_SRC)

#INCLUDES = -I/includes -I/get_next_line/includes -I./ft_printf/includes

COLOR_RESET = \033[0m
COLOR_DELETE = \033[0;31m
COLOR_PENDING = \033[01;33m
COLOR_SUCCESS = \033[1;32m
COLOR_DEFAULT = \033[1;34m

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJECTS)
	@echo "[$(COLOR_PENDING)Creating $@$(COLOR_RESET)]"
	@ar rc $(NAME) $(OBJECTS)
	@mv *.o $(OBJ_DIR)
	@mv $(FT_PRINTF_SRC_DIR)*.o $(OBJ_DIR)
	@mv $(GNL_DIR)*.o $(OBJ_DIR)
	@ranlib $(NAME)
	@echo "\033[A\033[K\033[A"
	@echo "Libft: [$(COLOR_SUCCESS)Object files created$(COLOR_RESET)]"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "Libft: [$(COLOR_SUCCESS)$(OBJ_DIR) was created$(RESET)]"

# $(OBJ_DIR)%.o: $(SRCS)%.c $(HEADERS)
# 	@echo "$(NAME)		[$(COLOR_PENDING)Compiling...$(COLOR_RESET)]"
# 	@$(CC) $(FLAGS) -I$(HEADERS) -c $< -o $@
# 	@echo "\033[A\033[K\033[A"
# 	@echo "$(NAME)		[$(COLOR_SUCCESS)OK$(COLOR_RESET)]"
# 	@echo "[$(COLOR_SUCCESS)FINISHED$(COLOR_RESET)]"

norme:
	norminette .

run:
	@$(CC) $(FLAGS) -I . -c main.c
	@$(CC) $(FLAGS) -o run main.o libft.a

clean:
	@echo "Libft: [$(COLOR_DELETE)Object files were removed$(COLOR_RESET)]"
	@rm -rf $(OBJ_DIR)
	@echo "Libft: [$(COLOR_DELETE)Objects folder was removed$(COLOR_RESET)]"

fclean: clean
	@rm -f $(NAME)
	@echo "[$(COLOR_SUCCESS)$(NAME) is no more$(COLOR_RESET)]"

re: fclean all

.PHONY: all clean fclean re