NAME := miniRT
CFLAGS := -fsanitize=leak -g
MLXFLAGS := -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
SRCS_DIR := ./srcs
OBJS_DIR := ./objs
MLX_DIR := ./mlx_linux

SRCS := $(shell find $(SRCS_DIR)/ -name "*.c" -type f | xargs)
OBJS := $(patsubst $(SRCS_DIR)%, $(OBJS_DIR)%, $(SRCS:.c=.o))

RM := rm -rf

all: $(NAME)


$(NAME): $(OBJS)
	$(MAKE) -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(MLXFLAGS) -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	mkdir -p $(OBJS_DIR)/$(*D)
	$(CC) $(CFLAGS) -I/usr/include -Imlx_linux -O3 -c $< -o $@

clean:
	$(MAKE) clean -C $(MLX_DIR)
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
