# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcuello <mcuello@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/25 21:31:23 by mcuello           #+#    #+#              #
#    Updated: 2026/08/12 17:52:02 by mcuello          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D

SRC			= main.c \
			  src/extension_rules.c \
			  src/fd_reader.c \
			  src/fd_parser.c \
			  src/fd_parser_aux.c \
			  src/fd_map_parser.c \
			  src/map_validations.c \
			  src/set_game.c \
			  src/aux.c \
			  src/get_next_line.c \
			  src/get_next_line_utils.c \
			  src/mlx_init_game.c \
			  src/loop_func.c \
			  src/hook.c \
			  src/ray_casting.c \
			  src/ray_collision.c \

OBJ			= $(SRC:.c=.o)

CC			= cc

CFLAGS		= -Wall -Wextra -Werror -g -Iinclude -Imlx -Ilibft

MLX_DIR		= MLX42

MLX_LIB		= $(MLX_DIR)/build/libmlx42.a

MLX_FLAGS	= -LMLX42/build -lmlx42 -ldl -lglfw -pthread -lm

MLX_INC		= -I$(MLX_DIR)/include

LIB_DIR		= libft

LIB		= $(LIB_DIR)/libft.a

all: $(LIB) $(MLX_LIB) $(NAME)

$(LIB):
	@make -C $(LIB_DIR) --no-print-directory

$(MLX_LIB):
	cd $(MLX_DIR) && cmake -B build && cmake --build build -j4

$(NAME): $(OBJ)
	$(CC) $(OBJ) -L$(LIB_DIR) -lft $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(MLX_INC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -rf $(MLX_DIR)/build
	make -C $(LIB_DIR) clean --no-print-directory

re: fclean all
