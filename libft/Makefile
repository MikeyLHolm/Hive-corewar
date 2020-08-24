# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 18:05:28 by sadawi            #+#    #+#              #
#    Updated: 2020/08/12 19:35:29 by sadawi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CLIBFT = libft/ft_atoi.c libft/ft_bzero.c libft/ft_isalnum.c \
libft/ft_isalpha.c libft/ft_isascii.c libft/ft_isdigit.c libft/ft_isprint.c \
libft/ft_itoa.c libft/ft_lstadd.c libft/ft_lstdel.c libft/ft_lstdelone.c \
libft/ft_lstiter.c libft/ft_lstmap.c libft/ft_lstnew.c libft/ft_memalloc.c \
libft/ft_memccpy.c libft/ft_memchr.c libft/ft_memcmp.c libft/ft_memcpy.c \
libft/ft_memdel.c libft/ft_memmove.c libft/ft_memset.c libft/ft_putchar.c \
libft/ft_putchar_fd.c libft/ft_putendl.c libft/ft_putendl_fd.c \
libft/ft_putnbr.c libft/ft_putnbr_fd.c libft/ft_putstr.c libft/ft_putstr_fd.c \
libft/ft_strcat.c libft/ft_strchr.c libft/ft_strclr.c libft/ft_strcmp.c \
libft/ft_strcpy.c libft/ft_strdel.c libft/ft_strdup.c libft/ft_strequ.c \
libft/ft_striter.c libft/ft_striteri.c libft/ft_strjoin.c libft/ft_strlcat.c \
libft/ft_strlen.c libft/ft_strmap.c libft/ft_strmapi.c libft/ft_strncat.c \
libft/ft_strncmp.c libft/ft_strncpy.c libft/ft_strnequ.c libft/ft_strnew.c \
libft/ft_strnstr.c libft/ft_strrchr.c libft/ft_strsplit.c libft/ft_strstr.c \
libft/ft_strsub.c libft/ft_strtrim.c libft/ft_tolower.c libft/ft_toupper.c \
libft/ft_putlst.c libft/ft_lstevery.c libft/ft_strfill.c \
libft/ft_strsplitlst.c libft/ft_swap.c libft/ft_longlen.c libft/ft_foreach.c \
libft/ft_putlong.c libft/ft_ulonglen.c libft/ft_putulong.c \
libft/ft_itoa_base.c libft/ft_itoa_base_low.c libft/ft_itoa_base_ul.c \
libft/ft_itoa_base_ul_low.c libft/ft_putdouble.c libft/ft_itoa_double.c \
libft/ft_chartostr.c libft/ft_atoilong.c libft/ft_strjoinfree.c \
libft/ft_isdigit_neg.c libft/ft_strcatfree.c
CFT_PRINTF = ft_printf/ft_printf.c ft_printf/handle_digit_datatypes.c \
ft_printf/handle_string_datatypes.c ft_printf/handle_flags.c \
ft_printf/handle_format_flags.c ft_printf/handle_width.c \
ft_printf/handle_char.c ft_printf/handle_data.c
CGET_NEXT_LINE = get_next_line/get_next_line.c
SRCS= $(addprefix srcs/, $(CLIBFT)) $(addprefix srcs/, $(CFT_PRINTF)) \
$(addprefix srcs/, $(CGET_NEXT_LINE))
OBJS=$(addprefix objs/, $(notdir $(SRCS:.c=.o)))
FLAGS=-Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(SRCS)
	@rm -rf objs
	@echo Compiling $(NAME)...
	@gcc $(FLAGS) -c -I includes $(SRCS)
	@mkdir objs
	@mv $(notdir $(SRCS:.c=.o)) objs
	@ar rc $(NAME) $(OBJS)
	@echo $(NAME) compiled succesfully!

clean:
	@/bin/rm -f $(OBJS)
	@rm -rf objs

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all
